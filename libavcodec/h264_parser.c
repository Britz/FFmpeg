/*
 * H.26L/H.264/AVC/JVT/14496-10/... parser
 * Copyright (c) 2003 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * H.264 / AVC / MPEG4 part10 parser.
 * @author Michael Niedermayer <michaelni@gmx.at>
 */

#define UNCHECKED_BITSTREAM_READER 1

#include "parser.h"
#include "h264data.h"
#include "golomb.h"
#include "h264_mvc.h"

#include <assert.h>


static int ff_h264_find_frame_end(H264Context *h, const uint8_t *buf, int buf_size)
{
    int i, j, extra_bytes;
    uint32_t state;
    ParseContext *pc = &(h->s.parse_context);
    int next_avc= h->is_avc ? 0 : buf_size;

//  printf("first %02X%02X%02X%02X\n", buf[0], buf[1],buf[2],buf[3]);
//    mb_addr= pc->mb_addr - 1;
    state= pc->state;
    extra_bytes = 0;
    if(state>13)
        state= 7;

    if(h->is_avc && !h->nal_length_size)
        av_log(h->s.avctx, AV_LOG_ERROR, "AVC-parser: nal length size invalid\n");

    for(i=0; i<buf_size; i++){
        if(i >= next_avc) {
            int nalsize = 0;
            i = next_avc;
            for(j = 0; j < h->nal_length_size; j++)
                nalsize = (nalsize << 8) | buf[i++];
            if(nalsize <= 0 || nalsize > buf_size - i){
                av_log(h->s.avctx, AV_LOG_ERROR, "AVC-parser: nal size %d remaining %d\n", nalsize, buf_size - i);
                return buf_size;
            }
            next_avc= i + nalsize;
            state= 5;
        }

        if(state==7){
#if HAVE_FAST_UNALIGNED
        /* we check i<buf_size instead of i+3/7 because its simpler
         * and there should be FF_INPUT_BUFFER_PADDING_SIZE bytes at the end
         */
#    if HAVE_FAST_64BIT
            while(i<next_avc && !((~*(const uint64_t*)(buf+i) & (*(const uint64_t*)(buf+i) - 0x0101010101010101ULL)) & 0x8080808080808080ULL))
                i+=8;
#    else
            while(i<next_avc && !((~*(const uint32_t*)(buf+i) & (*(const uint32_t*)(buf+i) - 0x01010101U)) & 0x80808080U))
                i+=4;
#    endif
#endif
            for(; i<next_avc; i++){
                if(!buf[i]){
                    state=2;
                    break;
                }
            }
        }else if(state<=2){
            if(buf[i]==1)   state^= 5; //2->7, 1->4, 0->5
            else if(buf[i]) state = 7;
            else            state>>=1; //2->1, 1->0, 0->0
        }else if(state<=5){
            int v= buf[i] & 0x1F;
            // EDIT JB parser find frame end
            if(v==6 || v==7 || v==8 || v==9){
//            if(v==6 || v==7 || v==8 || v==9 || v==15 || v==14 ){
//            	if(v==14){
//					i+=3;
//					extra_bytes = 3;
//				}
            // END EDIT
                if(pc->frame_start_found){
                    i++;
                    goto found;
                }
            // EDIT
            }else if(v==1 || v==2 || v==5){
//            }else if(v==1 || v==2 || v==3 || v==5 || v == 20 ){
//            	if(v==20){
//					i+=3;
//					extra_bytes = 3;
//            	}
            	// END EDIT
                state+=8;
                continue;
            }
            state= 7;
        }else{
            h->parse_history[h->parse_history_count++]= buf[i];
            if(h->parse_history_count>3){
                unsigned int mb, last_mb= h->parse_last_mb;
                GetBitContext gb;

                init_get_bits(&gb, h->parse_history, 8*h->parse_history_count);
                h->parse_history_count=0;
                mb= get_ue_golomb_long(&gb);
                last_mb= h->parse_last_mb;
                h->parse_last_mb= mb;
                if(pc->frame_start_found){
                    if(mb <= last_mb)
                        goto found;
                }else
                    pc->frame_start_found = 1;
                state= 7;
            }
        }
    }
    pc->state= state;
    if(h->is_avc)
        return next_avc;
    return END_NOT_FOUND;

found:
    pc->state=7;
    pc->frame_start_found= 0;
    if(h->is_avc)
        return next_avc;
    return i-(state&5) - 3*(state>7)-extra_bytes;
}

/**
 * Parse NAL units of found picture and decode some basic information.
 *
 * @param s parser context.
 * @param avctx codec context.
 * @param buf buffer with field/frame data.
 * @param buf_size size of the buffer.
 */
static inline int parse_nal_units(AVCodecParserContext *s,
                                  AVCodecContext *avctx,
                                  const uint8_t *buf, int buf_size)
{
    //H264Context *h = s->priv_data;
    const uint8_t *buf_end;
    int buf_size_intern = 0;
    unsigned int pps_id;
    unsigned int slice_type;
    int state = -1, view_id, buf_index = 0;
    const uint8_t *ptr;
    int q264 = buf_size >=4 && !memcmp("Q264", buf, 4);
    H264Context *h;

	for(view_id = 0; view_id<MAX_VIEW_COUNT; view_id++){
		ff_h264_extract_parser_Context(s, &h, view_id);
		buf_size_intern = ff_h264_find_mvc_frame_end(h, buf, buf_size-buf_index);

		buf_end = buf + buf_size_intern;

		/* set some sane default values */
		s->pict_type = AV_PICTURE_TYPE_I;
		s->key_frame = 0;

		h->s.avctx= avctx;
		h->sei_recovery_frame_cnt = -1;
		h->sei_dpb_output_delay         =  0;
		h->sei_cpb_removal_delay        = -1;
		h->sei_buffering_period_present =  0;

		// EDIT for MVC support
		// JB prefix NAL unit
		// @author: Jochen Britz
		h->prefix_nal_present = 0;

		// END EDIT

		if (!buf_size_intern)
			return 0;

		for(;;) {
			int src_length, dst_length, consumed, nalsize = 0;
			if (h->is_avc) {
				int i;
				if (h->nal_length_size >= buf_end - buf) break;
				nalsize = 0;
				for (i = 0; i < h->nal_length_size; i++)
					nalsize = (nalsize << 8) | *buf++;
				if (nalsize <= 0 || nalsize > buf_end - buf) {
					av_log(h->s.avctx, AV_LOG_ERROR, "AVC: nal size %d\n", nalsize);
					break;
				}
				src_length = nalsize;
			} else {
			buf = avpriv_mpv_find_start_code(buf, buf_end, &state);
			if(buf >= buf_end)
				break;
			--buf;
			src_length = buf_end - buf;
			}
			switch (state & 0x1f) {
			case NAL_SLICE:
			case NAL_IDR_SLICE:
				// Do not walk the whole buffer just to decode slice header
				if (src_length > 20)
					src_length = 20;
				break;
			// EDIT for MVC support
			// JB  NAL_SUB_SPS, NAL_PREFIX and NAL_EXT_SLICE
			// @author: Jochen Britz
			case NAL_EXT_SLICE:
				if (src_length > 23){
					src_length = 23;
				}
				break;
			// END EDIT
			}

			ptr= ff_h264_decode_nal(h, buf, &dst_length, &consumed, src_length);
			if (ptr==NULL || dst_length < 0)
				break;

			// EDIT for MVC support
			// JB prefix NAL unit
			// @author: Jochen Britz
			// av_log(h->s.avctx, AV_LOG_ERROR, "NAL unit type is %d.\n", h->nal_unit_type);

			if(ff_h264_mvc_deploy_nal_header(h) < 0){
				  av_log(h->s.avctx, AV_LOG_ERROR, "Error while setting prefix NAL unit values.\n");
				 // return -1;
			}

			// END EDIT

			init_get_bits(&h->s.gb, ptr, 8*dst_length);
			switch(h->nal_unit_type) {
			case NAL_SPS:
				ff_h264_decode_seq_parameter_set(h);
				break;
			case NAL_PPS:
				ff_h264_decode_picture_parameter_set(h, h->s.gb.size_in_bits);
				break;
			case NAL_SEI:
				ff_h264_decode_sei(h);
				break;

			// EDIT for MVC support
			// JB  NAL_SUB_SPS, NAL_PREFIX and NAL_EXT_SLICE
			// @author: Jochen Britz
			case NAL_SUB_SPS:
				ff_h264_decode_sub_sps(h, h->s.gb.size_in_bits);
				break;
			case NAL_PREFIX:
				ff_h264_decode_nal_prefix(h, src_length);
				break;
			case NAL_EXT_SLICE:
				if (h->svc_extension_flag) {
					/* specified in Annex G */
					ff_h264_svc_decode_slice_header(h);
					if (!h->slice_skip_flag) {
						/* specified in Annex G */
						//slice_data_in_scalable_extension();
					}
				} else {
					//ff_h264_decode_slice_header(h);
					//decode_slice(h);
					//ff_h264_decode_slice_data(h);
					goto nal_slice;
				}
				break;
			// END EDIT
			case NAL_IDR_SLICE:
				s->key_frame = 1;
				/* fall through */
			case NAL_SLICE:
		// EDIT for MVC support
		// JB 7.3.2.13 Slice layer extension
		// @author: Jochen Britz
		nal_slice:
		// END EDIT
				get_ue_golomb_long(&h->s.gb);  // skip first_mb_in_slice
				slice_type = get_ue_golomb_31(&h->s.gb);
				s->pict_type = golomb_to_pict_type[slice_type % 5];
				if (h->sei_recovery_frame_cnt >= 0) {
					/* key frame, since recovery_frame_cnt is set */
					s->key_frame = 1;
				}
				pps_id = get_ue_golomb(&h->s.gb);

				// EDIT for MVC support
				// JB sub_sps_buffer
				// @author: Jochen Britz
				// activate SPS and PPS in H264Context
				get_PPS(h, h,pps_id, 1);
				get_SPS(h, h ,h->pps.sps_id, 1);
				// set previous parsed values
				h->sps.pps_id = pps_id;
				//h->sps.first_mb_in_slice = first_mb_in_slice;

				// activate_PPS(h, pps_id);
				// if(pps_id>=MAX_PPS_COUNT) {
				//     av_log(h->s.avctx, AV_LOG_ERROR, "pps_id out of range\n");
				//     return -1;
				// }
				// if(!h->pps_buffers[pps_id]) {
				//     av_log(h->s.avctx, AV_LOG_ERROR, "non-existing PPS referenced\n");
				//     return -1;
				// }
				// h->pps= *h->pps_buffers[pps_id];
				// if(!h->sps_buffers[h->pps.sps_id]) {
				//     av_log(h->s.avctx, AV_LOG_ERROR, "non-existing SPS referenced\n");
				//     return -1;
				// }
				// h->sps = *h->sps_buffers[h->pps.sps_id];
				// END EDIT

				h->frame_num = get_bits(&h->s.gb, h->sps.log2_max_frame_num);

				avctx->profile = ff_h264_get_profile(&h->sps);
				avctx->level   = h->sps.level_idc;

				if(h->sps.frame_mbs_only_flag){
					h->s.picture_structure= PICT_FRAME;
				}else{
					// JB field_pic_flag & bottom_field_flag parsing
					// EDIT for MVC support
					// @author: Jochen Britz
					if (!h->sps.direct_8x8_inference_flag && slice_type == AV_PICTURE_TYPE_B) {
						av_log(h->s.avctx, AV_LOG_ERROR, "This stream was generated by a broken encoder, invalid 8x8 inference\n");
						return -1;
					}
					h->sps.field_pic_flag = get_bits1(&h->s.gb);
					if (h->sps.field_pic_flag) { // field_pic_flag

						// save field_pic_flag and bottom_field_flag for MVC processing
						// and bit stream conformance check  (H.8.1)
						h->sps.bottom_field_flag = get_bits1(&h->s.gb);
						h->s.picture_structure= PICT_TOP_FIELD + h->sps.bottom_field_flag;
						if(h->sps.bottom_field_flag){
							// JB TODO TopFieldOrderCnt have to be constant for this access unit for MCV streams

						}else{
							// JB TODO BottomFieldOrderCnt have to be constant for this access unit for MCV streams

						}
					} else {
						h->s.picture_structure= PICT_FRAME;
					}

					// previous version
	//                if(get_bits1(&h->s.gb)) { //field_pic_flag
	//                    h->s.picture_structure= PICT_TOP_FIELD + get_bits1(&h->s.gb); //bottom_field_flag
	//                } else {
	//                    h->s.picture_structure= PICT_FRAME;
	//                }
					// END EDIT
				}

				if(h->sps.pic_struct_present_flag) {
					switch (h->sei_pic_struct) {
						case SEI_PIC_STRUCT_TOP_FIELD:
						case SEI_PIC_STRUCT_BOTTOM_FIELD:
							s->repeat_pict = 0;
							break;
						case SEI_PIC_STRUCT_FRAME:
						case SEI_PIC_STRUCT_TOP_BOTTOM:
						case SEI_PIC_STRUCT_BOTTOM_TOP:
							s->repeat_pict = 1;
							break;
						case SEI_PIC_STRUCT_TOP_BOTTOM_TOP:
						case SEI_PIC_STRUCT_BOTTOM_TOP_BOTTOM:
							s->repeat_pict = 2;
							break;
						case SEI_PIC_STRUCT_FRAME_DOUBLING:
							s->repeat_pict = 3;
							break;
						case SEI_PIC_STRUCT_FRAME_TRIPLING:
							s->repeat_pict = 5;
							break;
						default:
							s->repeat_pict = h->s.picture_structure == PICT_FRAME ? 1 : 0;
							break;
					}
				} else {
					s->repeat_pict = h->s.picture_structure == PICT_FRAME ? 1 : 0;
				}
				break;
				//return 0; /* no need to evaluate the rest */
			}
			buf_index += h->is_avc ? nalsize : consumed;
			buf += h->is_avc ? nalsize : consumed;
		}

	}
    if (q264)
        return 0;
    /* didn't find a picture! */
    av_log(h->s.avctx, AV_LOG_ERROR, "missing picture in access unit with size %d\n", buf_size_intern);
    return -1;
}

static int h264_parse(AVCodecParserContext *s,
                      AVCodecContext *avctx,
                      const uint8_t **poutbuf, int *poutbuf_size,
                      const uint8_t *buf, int buf_size)
{
    //H264Context *h = s->priv_data;
    //ParseContext *pc = &h->s.parse_context;
	H264Context *h;
	ParseContext *pc;
	int next;

	ff_h264_extract_parser_Context(s, &h, 0);
	pc = &h->s.parse_context;

    if (!h->got_first) {
        h->got_first = 1;
        if (avctx->extradata_size) {
            h->s.avctx = avctx;
            // must be done like in decoder, otherwise opening the parser,
            // letting it create extradata and then closing and opening again
            // will cause has_b_frames to be always set.
            // Note that estimate_timings_from_pts does exactly this.
            if (!avctx->has_b_frames)
                h->s.low_delay = 1;
            ff_h264_decode_extradata(h, avctx->extradata, avctx->extradata_size);
        }
    }

    if(s->flags & PARSER_FLAG_COMPLETE_FRAMES){
        next= buf_size;
    }else{
        next= ff_h264_find_frame_end(h, buf, buf_size);

        if (ff_combine_frame(pc, next, &buf, &buf_size) < 0) {
            *poutbuf = NULL;
            *poutbuf_size = 0;
            return buf_size;
        }

        if(next<0 && next != END_NOT_FOUND){
            assert(pc->last_index + next >= 0 );
            ff_h264_find_frame_end(h, &pc->buffer[pc->last_index + next], -next); //update state
        }
    }

    parse_nal_units(s, avctx, buf, buf_size);

    if (h->sei_cpb_removal_delay >= 0) {
        s->dts_sync_point    = h->sei_buffering_period_present;
        s->dts_ref_dts_delta = h->sei_cpb_removal_delay;
        s->pts_dts_delta     = h->sei_dpb_output_delay;
    } else {
        s->dts_sync_point    = INT_MIN;
        s->dts_ref_dts_delta = INT_MIN;
        s->pts_dts_delta     = INT_MIN;
    }

    if (s->flags & PARSER_FLAG_ONCE) {
        s->flags &= PARSER_FLAG_COMPLETE_FRAMES;
    }

    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return next;
}

static int h264_split(AVCodecContext *avctx,
                      const uint8_t *buf, int buf_size)
{
    int i;
    uint32_t state = -1;
    int has_sps= 0;

    for(i=0; i<=buf_size; i++){
        if((state&0xFFFFFF1F) == 0x107)
            has_sps=1;
/*        if((state&0xFFFFFF1F) == 0x101 || (state&0xFFFFFF1F) == 0x102 || (state&0xFFFFFF1F) == 0x105){
        }*/
        if((state&0xFFFFFF00) == 0x100 && (state&0xFFFFFF1F) != 0x107 && (state&0xFFFFFF1F) != 0x108 && (state&0xFFFFFF1F) != 0x109){
            if(has_sps){
                while(i>4 && buf[i-5]==0) i--;
                return i-4;
            }
        }
        if (i<buf_size)
            state= (state<<8) | buf[i];
    }
    return 0;
}

static void close(AVCodecParserContext *s)
{
	//H264Context *h = s->priv_data;
	H264Context *h;
	ParseContext *pc;
	int view_id;
	for(view_id = 0; view_id<MAX_VIEW_COUNT; view_id++){
		ff_h264_extract_parser_Context(s, &h, view_id);
		pc = &h->s.parse_context;

		av_free(pc->buffer);
		ff_h264_free_context(h);
	}
}

static int init(AVCodecParserContext *s)
{
	//H264Context *h = s->priv_data;
	H264Context *h, *h0;
	int view_id;
	for(view_id = 0; view_id<MAX_VIEW_COUNT; view_id++){
		ff_h264_extract_parser_Context(s, &h, view_id);

		if(view_id == 0){
			h0 = h;
		}

		h0->mvc_context[view_id] = h;

		h->thread_context[0] = h;
		h->s.slice_context_count = 1;
	}
	for(view_id = 1; view_id<MAX_VIEW_COUNT; view_id++){
		ff_h264_extract_parser_Context(s, &h, view_id);
		if(view_id != 0){
			memcpy(h->mvc_context, h0->mvc_context, sizeof(h0->mvc_context));
		}
	}
    return 0;
}

AVCodecParser ff_h264_parser = {
    .codec_ids      = { CODEC_ID_H264 },
    .priv_data_size = (sizeof(H264Context)*MAX_VIEW_COUNT),
    .parser_init    = init,
    .parser_parse   = h264_parse,
    .parser_close   = close,
    .split          = h264_split,
};
