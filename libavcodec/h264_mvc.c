/*
 * h264_mvc.c
 * Copyright (c) 2012 Jochen Britz
 */

/**
 * @file
 * H.264 / AVC / MPEG4 part10 codec / MVC extension.
 * @author: Jochen Britz
 */

#include "h264_mvc.h"

#include "libavutil/imgutils.h"
#include "internal.h"
#include "avcodec.h"
#include "golomb.h"
#include "math.h"

// include source files for access to static inline functions
#include "h264_refs.c"
#include "h264_ps.c"
// #include "h264.c"

/** MIN */
int min(int a, int b) {
	return (a < b) ? a : b;
}

/** MAX */
int max(int a, int b) {
	return (a > b) ? a : b;
}

MpegEncContext* ff_h264_extract_Context(const AVCodecContext * avctx, H264Context** h, int voidx){
	H264Context *h_mvc = avctx->priv_data;
	if(voidx < 0 && voidx > MAX_VIEW_COUNT){
		(*h) = &h_mvc[0];
		(*h)->voidx = 0;
		//av_log((*h)->s.avctx, AV_LOG_ERROR,"ff_h264_extract_Context: voidx %d out of range [0..%d] took H264Context[0] instead.\n", voidx, MAX_VIEW_COUNT);
	}else{
		(*h) = &h_mvc[voidx];
		(*h)->voidx = voidx;
		//av_log((*h)->s.avctx, AV_LOG_INFO,"ff_h264_extract_Context: H264Context[%d] extracted.\n", voidx);
	}
	init_H264Context(*h);
	return &(*h)->s;
}

MpegEncContext* ff_h264_extract_parser_Context(const AVCodecParserContext* avctx, H264Context** h, int voidx){
	H264Context *h_mvc = avctx->priv_data;
	if(voidx < 0 && voidx > MAX_VIEW_COUNT){
		(*h) = &h_mvc[0];
		(*h)->voidx = 0;
		//av_log((*h)->s.avctx, AV_LOG_ERROR,"ff_h264_extract_Context: voidx %d out of range [0..%d] took H264Context[0] instead.\n", voidx, MAX_VIEW_COUNT);
	}else{
		(*h) = &h_mvc[voidx];
		(*h)->voidx = voidx;
		//av_log((*h)->s.avctx, AV_LOG_INFO,"ff_h264_extract_Context: H264Context[%d] extracted.\n", voidx);
	}
	init_H264Context(*h);
	return &(*h)->s;
}

MpegEncContext* ff_h264_get_MpegEncContext(H264Context *h){
	if(h->mvc_context[0]){
		return &h->mvc_context[0]->s;
	}else{
		return &h->s;
	}
}

void init_H264Context(H264Context *h){
}


int save_PPS(H264Context *h, PPS* pps, uint pps_id, uint8_t activate_it){
	int ret = 0;
	H264Context *h_main = h->mvc_context[0];
	if(!h_main){
		h_main = h;
	}
	if(pps_id > MAX_PPS_COUNT){
		av_log(h->s.avctx, AV_LOG_ERROR, "pps_id (%u) out of range [0..%d]\n", pps_id, MAX_PPS_COUNT);
		return -1;
	}
	if(h_main->pps_buffers[pps_id]){
		av_free(h_main->pps_buffers[pps_id]);
		ret = 1;
	}
	h_main->pps_buffers[pps_id] = pps;
	if(activate_it){
		int i;
		h->pps = *pps;
		for(i = 0; i<MAX_VIEW_COUNT; i++){
			if(h->mvc_context[i] && h->mvc_context[i] != h){
				h->mvc_context[i]->pps = *pps;
			}
		}
		av_log(h->s.avctx, AV_LOG_INFO, "PPS (%u) activated.\n", pps_id);
	}
	return ret;
}

int save_SPS(H264Context *h, SPS* sps, uint8_t activate_it){
	int  ret = 0;
	H264Context *h_main = h->mvc_context[0];
	if(!h_main){
		h_main = h;
	}
	if(h->nal_unit_type == NAL_SUB_SPS){
		sps->is_sub_sps = 1;
		if(h_main->sub_sps_buffers[sps->id]){
			av_free(h_main->sub_sps_buffers[sps->id]);
			ret = 1;
		}
		h_main->sub_sps_buffers[sps->id] = sps;
		/* Do not automatically activate SPS.
		   Subset SPS are activated through MVC SEI messages
		   or NAL units with type 20. (NAL_EXT_SLICE) */
	}else if(h->nal_unit_type ==NAL_SPS || h->nal_unit_type == NAL_SPS_EXT){
		sps->is_sub_sps = 0;
		if(h_main->sps_buffers[sps->id]){
			av_free(h_main->sps_buffers[sps->id]);
			ret = 1;
		}
		h_main->sps_buffers[sps->id] = sps;
	}else{
		return -1;
	}
	// activate SPS
	if(sps && activate_it){
		int i;
		h->sps = *sps;
		for(i = 0; i<MAX_VIEW_COUNT; i++){
			if(h->mvc_context[i] && h->mvc_context[i] != h){
				h->mvc_context[i]->sps = *sps;
			}
		}
		av_log(h->s.avctx, AV_LOG_INFO, "save_SPS: %sSPS (%u) activated.\n",sps->is_sub_sps ? "Subset " : "", sps->id);
	}
	return ret;
}

SPS* get_SPS(H264Context *h0, H264Context *h, uint sps_id, uint8_t activate_it){
	H264Context *h_main = h0->mvc_context[0];
	SPS *sps = 0;

	if(!h_main){
		h_main = h0;
	}
	if (h->nal_unit_type == NAL_EXT_SLICE || h->nal_unit_type == NAL_SUB_SPS  ) { // MVC slice -> use sub_sps_buffer

		if(h->sps.id == sps_id && h->sps.is_sub_sps){
			return &h->sps;
		}
		if(!h_main->sub_sps_buffers[sps_id]) {
			av_log(h->s.avctx, AV_LOG_ERROR,
					"Non-existing subset SPS (%u) referenced, try SPS instead.\n", sps_id);
			// Try normal sps buffer
			if (!h_main->sps_buffers[sps_id]) {
				av_log(h->s.avctx, AV_LOG_ERROR,
						"Non-existing subset SPS (%u) referenced, also SPS (%u) is non-existing.\n",
						sps_id, sps_id);
				return 0;
			}
			sps = h_main->sps_buffers[sps_id];
		}else{
			sps = h_main->sub_sps_buffers[sps_id];
		}
	} else {  // normal slice -> use sps_buffer
		if(h->sps.id == sps_id && !h->sps.is_sub_sps){
			return &h->sps;
		}
		if (!h_main->sps_buffers[sps_id]) {
			if(!h->nal_unit_type == NAL_PPS){
				av_log(h->s.avctx, AV_LOG_ERROR, "Non-existing SPS (%u) referenced, try subset SPS instead.\n", sps_id);
			}
			// Try normal sps buffer
			if (!h_main->sub_sps_buffers[sps_id]) {
				if(!h->nal_unit_type == NAL_PPS){
					av_log(h->s.avctx, AV_LOG_ERROR,"Non-existing SPS (%u) referenced, also subset SPS (%u) is non-existing.\n", sps_id, sps_id);
				}else{
					av_log(h->s.avctx, AV_LOG_ERROR,"Non-existing SPS (%u) or subset SPS (%u) referenced.\n", sps_id, sps_id);
				}
				return 0;
			}
			sps = h_main->sub_sps_buffers[sps_id];
		}else{
			sps = h_main->sps_buffers[sps_id];
		}
	}
	if(sps  && activate_it){
		int i;
		if(h->sps.id == sps->id && h->sps.is_sub_sps == sps->is_sub_sps){
			return &h->sps;
		}
		h->sps = *sps;
		for(i = 0; i<MAX_VIEW_COUNT; i++){
			if(h0->mvc_context[i] && h0->mvc_context[i] != h0){
				h0->mvc_context[i]->sps = *sps;
			}
		}
		if(h != h0){
			// write to main context can lead to memory error
			// h0->sps = *sps;
		}
		av_log(h->s.avctx, AV_LOG_INFO, "get_SPS: %sSPS (%u) activated.\n",sps->is_sub_sps ? "Subset " : "", sps_id);
		return &h->sps;
	}
	return sps;
}

PPS* get_PPS(H264Context *h0, H264Context *h, uint pps_id, uint8_t activate_it){
	H264Context *h_main = h0->mvc_context[0];
	PPS *pps = 0;

	if(!h_main){
		h_main = h0;
	}

	if (pps_id >= MAX_PPS_COUNT) {
		av_log(h->s.avctx, AV_LOG_ERROR, "pps_id %u out of range\n", pps_id);
		return 0;
	}
	if (!h_main->pps_buffers[pps_id]) {
		av_log(h->s.avctx, AV_LOG_ERROR, "Non-existing PPS (%u) referenced.\n", pps_id);
		return 0;
	}
	pps = h_main->pps_buffers[pps_id];
	if(pps  && activate_it){
		int i;
		h->pps = *pps;
		for(i = 0; i<MAX_VIEW_COUNT; i++){
			if(h0->mvc_context[i] && h0->mvc_context[i] != h0){
				h0->mvc_context[i]->pps = *pps;
			}
		}
		if(h != h0){
			// write to main context can lead to memory error
			// should be done on update_thread_context
			// h0->pps = *pps;
		}
		//av_log(h->s.avctx, AV_LOG_INFO, "PPS (%u) activated.\n", pps_id);
		return &h->pps;
	}
	return pps;
}

int ff_h264_find_mvc_frame_end(H264Context *h, const uint8_t *buf, int buf_size){
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
            if(v==6 || v==7 || v==8 || v==9 || v==15 || v==14 ){
            	if(v==14){
					i+=3;
					extra_bytes = 3;
				}
                if(pc->frame_start_found){
                    i++;
                    goto found;
                }
            }else if(v==1 || v==2 || v==3 || v==5 || v == 20 ){
            	if(v==20){
					i+=3;
					extra_bytes = 3;
            	}
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


/** 7.3.2.1.2 */
void ff_h264_decode_sps_ext(H264Context *h, SPS *sps) {
	MpegEncContext * const s = &h->s;
	sps->seq_parameter_set_id = get_se_golomb(&s->gb);
	sps->aux_format_idc = get_se_golomb(&s->gb);
	if (sps->aux_format_idc != 0) {
		sps->bit_depth_aux_minus8 = get_se_golomb(&s->gb);
		sps->alpha_incr_flag = get_bits1(&s->gb);
		sps->alpha_opaque_value = get_bits(&s->gb,
				sps->bit_depth_aux_minus8 + 9);
		sps->alpha_transparent_value = get_bits(&s->gb,
				sps->bit_depth_aux_minus8 + 9);
	}
	sps->additional_extension_flag = get_bits1(&s->gb);
	ff_h264_decode_trailing_bits(h);
}

/** 7.3.2.1.3 */
int ff_h264_decode_sub_sps(H264Context *h, int bit_length) {
	MpegEncContext * const s = &h->s;
	int bits_left,sps_id;
	SPS *sps;

	sps_id  = ff_h264_decode_seq_parameter_set(h);

	if (sps_id < 0) {
		return -1;
	}

	sps  = get_SPS(h, h, sps_id, 0);

	if (sps->profile_idc == 83 || sps->profile_idc == 86) {
		ff_h264_svc_decode_sps(h, sps); /* specified in Annex G */
		sps->svc_vui_parameters_present_flag = get_bits1(&s->gb);
		if (sps->svc_vui_parameters_present_flag == 1) {
			//svc_vui_parameters_extension(h) /* specified in Annex G */
		}
	} else if ( sps->profile_idc == 118 || sps->profile_idc == 128) {
		sps->bit_equal_to_one = get_bits1(&s->gb); /* equal to 1 */
 		ff_h264_mvc_decode_sps(h, sps); /* specified in Annex H */
		sps->mvc_vui_parameters_present_flag = get_bits1(&s->gb);
		if (sps->mvc_vui_parameters_present_flag == 1) {
			if (ff_h264_mvc_decode_vui_parameters(h, sps) < 0) /* specified in Annex H */
				return -1;
		}
	}
	sps->additional_extension2_flag = get_bits1(&s->gb);
	if (sps->additional_extension2_flag == 1) {
		//		Ignore, bits are reserved for future extensions
		bits_left = bit_length - get_bits_count(&s->gb);
		while (bits_left > 0) {
			sps->additional_extension2_data_flag = get_bits1(&s->gb);
			bits_left = bit_length - get_bits_count(&s->gb);
		}
	}
	return 0;
}

/** 7.3.2.10 */
int ff_h264_decode_slice_trailing_bits(H264Context *h) {
	MpegEncContext * const s = &h->s;
	uint cabac_zero_word;

	if (ff_h264_decode_trailing_bits(h)<0){
		av_log(h->s.avctx, AV_LOG_ERROR,
					"ff_h264_rbsp_slice_trailing_bits: ff_h264_rbsp_trailing_bits returned with error");
		return -1;
	}
	if( h->pps.cabac  ){ /* entropy_coding_mode_flag */
		while( get_bits_left(&s->gb) ){
			cabac_zero_word = get_bits(&s->gb,16); 	/* equal to 0x0000 */
			if (cabac_zero_word != 0x0000){
				av_log(h->s.avctx, AV_LOG_ERROR,
							"ff_h264_rbsp_slice_trailing_bits: cabac_zero_word has to be equal to 0x0000 but is %#x\n", cabac_zero_word);
				return -1;
			}
		}
	}
	return 0;
}

/** 7.3.2.11 */
int ff_h264_decode_trailing_bits(H264Context *h) {
	MpegEncContext * const s = &h->s;
	u_int8_t rbsp_alignment_zero_bit;
	u_int8_t rbsp_stop_one_bit = get_bits1(&s->gb); /* equal to 1 */
	if (rbsp_stop_one_bit != 1){
		av_log(h->s.avctx, AV_LOG_ERROR,
					"ff_h264_rbsp_trailing_bits: rbsp_stop_one_bit has to be equal to 1 but is %d\n", rbsp_stop_one_bit);
		return -1;
	}
	while( get_bits_count(&s->gb) % 7 ){
		rbsp_alignment_zero_bit = get_bits1(&s->gb); /* equal to 0 */
		if (rbsp_alignment_zero_bit){
			av_log(h->s.avctx, AV_LOG_ERROR,
						"ff_h264_rbsp_trailing_bits: rbsp_alignment_zero_bit has to be equal to 0 but is %d\n", rbsp_alignment_zero_bit);
			return -1;
		}
	}
	//align_get_bits(&s->gb);
	return 0;
}

/** 7.3.2.12 */
void ff_h264_decode_nal_prefix_threaded(H264Context *h, H264Context *h0, int bit_length) {

/* == FFmpeg specific things, not in the specification == */
	// set present flag
	h0->prefix_nal_present = 1;
	// copy header prefix NAL unit specific values
	// from thread context to main context
	h0->non_idr_present = h->non_idr_present;
	h0->nal_ref_idc  = h->nal_ref_idc,
	h0->idr_flag  = h->idr_flag,
	h0->priority_id = h->priority_id;
	h0->view_id = h->view_id;
	h0->temporal_id = h->temporal_id;
	h0->anchor_pic_flag = h->anchor_pic_flag;
	h0->inter_view_flag = h->inter_view_flag;

/* == Continue with specification == */
	if (h0->svc_extension_flag) {
		ff_h264_svc_decode_nal_prefix(h0, bit_length);
	}
}

void ff_h264_decode_nal_prefix(H264Context *h, int bit_length) {
		/* == FFmpeg specific things, not in the specification == */
		// set present flag
		h->prefix_nal_present = 1;


		/* == Continue with specification == */
		if (h->svc_extension_flag) {
			ff_h264_svc_decode_nal_prefix(h, bit_length);
		}
}



/** 7.3.2.13 */
void ff_h264_decode_slice_ext(H264Context *h) {
	//MpegEncContext * const s = &h->s;
	av_log(h->s.avctx, AV_LOG_DEBUG, "ff_h264_decode_slice_ext\n");
	if (h->svc_extension_flag) {
		/* specified in Annex G */
		ff_h264_svc_decode_slice_header(h);
		if (!h->slice_skip_flag) {
			/* specified in Annex G */
			//slice_data_in_scalable_extension();
		}
	} else {
		ff_h264_decode_slice_header(h);
		//decode_slice(h);
		//ff_h264_decode_slice_data(h);
	}
	//ff_h264_decode_slice_trailing_bits(h);
}

/** 7.3.3 */
int ff_h264_decode_slice_header(H264Context *h) {
	// also available as inline function h264_ps.c with more initializing

	PPS *pps;
	SPS *sps;
	unsigned int tmp, pps_id, first_mb_in_slice,slice_type;
	MpegEncContext * const s = &h->s;


	av_log(h->s.avctx, AV_LOG_DEBUG, "ff_h264_decode_slice_header\n");

	first_mb_in_slice = get_ue_golomb_long(&s->gb);

	slice_type = get_ue_golomb_31(&s->gb);
	if (slice_type > 9) {
		av_log(h->s.avctx, AV_LOG_ERROR, "slice type too large (%d) at %d %d\n",
				h->slice_type, s->mb_x, s->mb_y);
		return -1;
	}
	if (slice_type > 4) {
		slice_type -= 5;
		h->slice_type_fixed = 1;
	} else
		h->slice_type_fixed = 0;

	slice_type = golomb_to_pict_type[slice_type];

	h->slice_type = slice_type;
	h->slice_type_nos = slice_type & 3;

	pps_id = get_ue_golomb(&s->gb);

	// activate SPS and PPS in H264Context
	pps = get_PPS(h, h,pps_id, 1);
	sps = get_SPS(h, h ,h->pps.sps_id, 1);
	if(!sps || !pps ){
		av_log(h->s.avctx, AV_LOG_ERROR, "required PPS or SPS does not exist");
		return -1;
	}
	// set previous parsed values
	sps->pps_id = pps_id;
	sps->first_mb_in_slice = first_mb_in_slice;

	if( sps->residual_color_transform_flag == 1 ){
		sps->colour_plane_id = get_bits(&s->gb, 2);
	}

	h->frame_num = get_bits(&s->gb, sps->log2_max_frame_num);


	if (!sps->frame_mbs_only_flag) {
		// save field_pic_flag and bottom_field_flag for MVC processing
		sps->field_pic_flag = get_bits1(&s->gb);
		if (sps->field_pic_flag) {
			sps->bottom_field_flag = get_bits1(&s->gb);
			// do bit stream conformance check  (H.8.1)
			if (sps->bottom_field_flag) {
				// TopFieldOrderCnt have to be constant for this access unit for MCV streams
			} else {
				// BottomFieldOrderCnt have to be constant for this access unit for MCV streams
			}
		}
	}

	if (h->nal_unit_type == NAL_IDR_SLICE){ /* IdrPicFlag*/
		get_ue_golomb(&s->gb); /* idr_pic_id */
	}
	if (sps->poc_type == 0) { /* pic_order_cnt_type */
		h->poc_lsb = get_bits(&s->gb, sps->log2_max_poc_lsb); /* pic_order_cnt_lsb */

		//if( bottom_field_pic_order_in_frame_present_flag && !field_pic_flag )
		if (h->pps.pic_order_present == 1 && s->picture_structure == PICT_FRAME){
			h->delta_poc_bottom = get_se_golomb(&s->gb); /* delta_pic_order_cnt_bottom */

		}
	}

	//if( pic_order_cnt_type == 1 && !delta_pic_order_always_zero_flag )
	if (sps->poc_type == 1 && !sps->delta_pic_order_always_zero_flag) {
		h->delta_poc[0] = get_se_golomb(&s->gb); /* delta_pic_order_cnt[ 0 ] */

		// 	if( bottom_field_pic_order_in_frame_present_flag && !field_pic_flag )
		if (h->pps.pic_order_present == 1 && s->picture_structure == PICT_FRAME){
			h->delta_poc[1] = get_se_golomb(&s->gb); /* delta_pic_order_cnt[ 1 ]  */
		}
	}

	if (h->pps.redundant_pic_cnt_present){ /* redundant_pic_cnt_present_flag */
		h->redundant_pic_count = get_ue_golomb(&s->gb); /* redundant_pic_cnt */
	}

	if(h->slice_type == AV_PICTURE_TYPE_B ){
		h->direct_spatial_mv_pred = get_bits1(&s->gb); /* direct_spatial_mv_pred_flag */

		if (get_bits1(&s->gb)) { /* num_ref_idx_active_override_flag */
			h->ref_count[0] = get_ue_golomb(&s->gb) + 1; /* num_ref_idx_l0_active_minus1 + 1 */
			h->ref_count[1] = get_ue_golomb(&s->gb) + 1; /* num_ref_idx_l1_active_minus1 + 1 */
		}
	}else if(h->slice_type_nos == AV_PICTURE_TYPE_P){ /* slice_type == P || SP */
		if (get_bits1(&s->gb)) { /* num_ref_idx_active_override_flag */
					h->ref_count[0] = get_ue_golomb(&s->gb) + 1; /* num_ref_idx_l0_active_minus1 + 1 */
		}

	} else {
		 h->ref_count[1] = h->ref_count[0] = h->list_count = 0;
	}

	if (h->nal_unit_type == NAL_EXT_SLICE) {
		if (ff_h264_mvc_reorder_ref_pic_list(h, sps) < 0) {
			h->ref_count[1] = h->ref_count[0] = 0;
			av_log(s->avctx, AV_LOG_ERROR, "ff_h264_decode_mvc_ref_pic_list_reordering failed\n");
			return -1;
		}
	} else {
		if (ff_h264_decode_ref_pic_list_reordering(h) < 0) {
			h->ref_count[1] = h->ref_count[0] = 0;
			av_log(s->avctx, AV_LOG_ERROR, "ff_h264_decode_ref_pic_list_reordering failed\n");
			return -1;
		}
	}

	if ((h->pps.weighted_pred && h->slice_type_nos == AV_PICTURE_TYPE_P)
			|| (h->pps.weighted_bipred_idc == 1 && h->slice_type_nos == AV_PICTURE_TYPE_B)){
		pred_weight_table(h);
	}

	if (h->nal_ref_idc){
		if(ff_h264_decode_ref_pic_marking(h, &s->gb) < 0){ /* dec_ref_pic_marking() */
			return AVERROR_INVALIDDATA ;
		}
	}

	//if( entropy_coding_mode_flag && slice_type != I && slice_type != SI )
	if (h->slice_type_nos != AV_PICTURE_TYPE_I && h->pps.cabac) {
		tmp = get_ue_golomb_31(&s->gb);
		if (tmp > 2) {
			av_log(s->avctx, AV_LOG_ERROR, "cabac_init_idc overflow\n");
			return -1;
		}
		h->cabac_init_idc = tmp;
	}

	h->last_qscale_diff = 0;
	tmp = h->pps.init_qp + get_se_golomb(&s->gb); /* slice_qp_delta */
	if (tmp > 51 + 6 * (sps->bit_depth_luma - 8)) {
		av_log(s->avctx, AV_LOG_ERROR, "QP %u out of range\n", tmp);
		return -1;
	}
	s->qscale = tmp;
	h->chroma_qp[0] = get_chroma_qp(h, 0, s->qscale);
	h->chroma_qp[1] = get_chroma_qp(h, 1, s->qscale);



	if (h->slice_type == AV_PICTURE_TYPE_SP){
		get_bits1(&s->gb); /* sp_for_switch_flag */
	}
	if (h->slice_type == AV_PICTURE_TYPE_SP
		|| h->slice_type == AV_PICTURE_TYPE_SI){
		get_se_golomb(&s->gb); /* slice_qs_delta */
	}

	if (h->pps.deblocking_filter_parameters_present) {
		tmp = get_ue_golomb_31(&s->gb);
		if (tmp > 2) {
			av_log(s->avctx, AV_LOG_ERROR,
					"deblocking_filter_idc %u out of range\n", tmp);
			return -1;
		}
		h->deblocking_filter = tmp;
		if (h->deblocking_filter < 2)
			h->deblocking_filter ^= 1;  // 1<->0

		if (h->deblocking_filter) {
			h->slice_alpha_c0_offset += get_se_golomb(&s->gb) << 1;
			h->slice_beta_offset += get_se_golomb(&s->gb) << 1;
			if (h->slice_alpha_c0_offset > 104U
					|| h->slice_beta_offset > 104U) {
				av_log(s->avctx, AV_LOG_ERROR,
						"deblocking filter parameters %d %d out of range\n",
						h->slice_alpha_c0_offset, h->slice_beta_offset);
				return -1;
			}
		}
	}

	if(h->pps.slice_group_count > 1 &&
			h->pps.mb_slice_group_map_type >= 3 && h->pps.mb_slice_group_map_type <= 5){

		h->pps.slice_group_change_cycle =  get_bits(&s->gb,ceil(log2((sps->mb_width * sps->mb_height)/pps->slice_group_change_rate + 1 ) ));

	}
	return 0;
}

/**	7.3.4 */
void ff_h264_decode_slice_data(H264Context *h, SPS *sps) {
	MpegEncContext * const s = &h->s;
	int moreDataFlag, currMbAddr, prevMbSkipped, mb_skip_flag ,end_of_slice_flag;
	int i;

	av_log(h->s.avctx, AV_LOG_DEBUG, "ff_h264_decode_slice_data\n");

	if( h->pps.cabac ){ /* entropy_coding_mode_flag */
		align_get_bits(&s->gb);
	}
	currMbAddr = sps->first_mb_in_slice * ( 1 + h->mb_aff_frame );
	moreDataFlag = 1;
	prevMbSkipped = 0;
	while( moreDataFlag ) {
		if( h->slice_type_nos != AV_PICTURE_TYPE_I ){
			if( !h->pps.cabac ) {
				s->mb_skip_run = get_ue_golomb(&s->gb);
				prevMbSkipped = ( s->mb_skip_run > 0 );
				for( i=0; i<s->mb_skip_run; i++ ){
					//JB TODO currMbAddr = NextMbAddress( currMbAddr );
				}
				if(s->mb_skip_run > 0 ){
					moreDataFlag = get_bits_left(&s->gb);
				}
			} else {
				//JB TODO mb_skip_flag = ae(v);
				mb_skip_flag = 0;
				moreDataFlag = !mb_skip_flag;
			}
		}
		if( moreDataFlag ) {
			if( h->mb_aff_frame && /* MbaffFrameFlag */
					( currMbAddr % 2 == 0 ||
					( currMbAddr % 2 == 1 && prevMbSkipped ) ) ){
				// 9.3 CABAC parsing process for slice data
				// This process is invoked when parsing syntax elements with descriptor ae(v) in clauses 7.3.4 and 7.3.5 when
				// entropy_coding_mode_flag is equal to 1.
				if( h->pps.cabac ){
					// mb_field_decoding_flag = ae(v);
					//JB TODO h->mb_field_decoding_flag = ae(v);
				}else{
					// mb_field_decoding_flag = u(1);
					h->mb_field_decoding_flag = get_bits1(&s->gb);
				}
			}
			//macroblock_layer();
		}
		if( !h->pps.cabac ){
			moreDataFlag  = get_bits_left(&s->gb); /* more_rbsp_data() */
		} else {
			if(h->slice_type_nos != AV_PICTURE_TYPE_I){
				prevMbSkipped = mb_skip_flag;
			}
			if( h->mb_aff_frame && currMbAddr % 2 == 0 ){
				moreDataFlag = 1;
			} else {
				//JB TODO end_of_slice_flag = ae(v);
				end_of_slice_flag = 0;
				moreDataFlag = !end_of_slice_flag;
			}
		}
		//JB TODO currMbAddr = NextMbAddress( currMbAddr );
	}
}

/** 7.4.2.1.1 */
int ff_h264_chroma_array_type(H264Context *h, SPS *sps) {
	if (sps->residual_color_transform_flag == 1) {
		return sps->chroma_format_idc;
	} else {
		return 0;
	}
}

/**	G.7.3.1.1 */
void ff_h264_svc_decode_nal_header(H264Context *h) {
	// JB TODO &s->gb is not always initialized when this function is needed.
	//	create a second one how uses source and pointer
	//	as done in ff_h264_decode_nal_unit_header_mvc_extension!
	MpegEncContext * const s = &h->s;
	h->idr_flag = get_bits1(&s->gb);
	h->priority_id = get_bits(&s->gb, 6);
	h->no_inter_layer_pred_flag = get_bits1(&s->gb);
	h->dependency_id  = get_bits(&s->gb, 3);
	h->quality_id = get_bits(&s->gb, 4);
	h->temporal_id = get_bits(&s->gb, 3);
	h->use_ref_base_pic_flag = get_bits1(&s->gb);
	h->discardable_flag = get_bits1(&s->gb);
	h->output_flag = get_bits1(&s->gb);
	get_bits(&s->gb, 2); /* reserved_three_2bit */
}

/** G.7.3.2.1.4 */
void ff_h264_svc_decode_sps(H264Context *h, SPS *sps) {
	MpegEncContext * const s = &h->s;
	int chromaArrayType = ff_h264_chroma_array_type(h, sps);
	sps->inter_layer_deblocking_filter_control_present_flag = get_bits1(&s->gb);
	sps->extended_spatial_scalability_idc = get_bits(&s->gb, 2);
	if (chromaArrayType == 1 || chromaArrayType == 2) {
		sps->chroma_phase_x_plus1_flag = get_bits1(&s->gb);
	}
	if (chromaArrayType == 1) {
		sps->chroma_phase_y_plus1 = get_bits(&s->gb, 2);
	}
	if (sps->extended_spatial_scalability_idc == 1) {
		if (chromaArrayType > 0) {
			sps->seq_ref_layer_chroma_phase_x_plus1_flag = get_bits1(&s->gb);
			sps->seq_ref_layer_chroma_phase_y_plus1 = get_bits(&s->gb, 2);
		}
		sps->seq_scaled_ref_layer_left_offset = get_se_golomb(&s->gb);
		sps->seq_scaled_ref_layer_top_offset = get_se_golomb(&s->gb);
		sps->seq_scaled_ref_layer_right_offset = get_se_golomb(&s->gb);
		sps->seq_scaled_ref_layer_bottom_offset = get_se_golomb(&s->gb);
	}
	sps->seq_tcoeff_level_prediction_flag = get_bits1(&s->gb);
	if (sps->seq_tcoeff_level_prediction_flag) {
		sps->adaptive_tcoeff_level_prediction_flag = get_bits1(&s->gb);
	}
	sps->slice_header_restriction_flag = get_bits1(&s->gb);

}

/** G.7.3.2.12.1 */
void ff_h264_svc_decode_nal_prefix(H264Context *h, int bit_length) {
	MpegEncContext * const s = &h->s;

	int bits_left = 0;

	if (h->nal_ref_idc != 0) {
		h->store_ref_base_pic_flag = get_bits1(&s->gb);
		if ((h->use_ref_base_pic_flag || h->store_ref_base_pic_flag)
				&& !h->idr_flag) {
			ff_h264_decode_ref_base_pic_marking(h);
		}
		h->additional_prefix_nal_unit_extension_flag = get_bits1(&s->gb);

		bits_left = (bit_length - get_bits_count(&s->gb));
		if (h->additional_prefix_nal_unit_extension_flag == 1) {

			while (bits_left > 0) {
				h->additional_prefix_nal_unit_extension_data_flag = get_bits1(
						&s->gb);
				bits_left = bit_length - get_bits_count(&s->gb);
			}
			ff_h264_decode_trailing_bits(h);
		} else if (bits_left) {
			while (bits_left > 0) {
				h->additional_prefix_nal_unit_extension_data_flag = get_bits1(
						&s->gb);
				bits_left = bit_length - get_bits_count(&s->gb);
			}
			ff_h264_decode_trailing_bits(h);
		}
	}
}

/** G.7.3.3.4 */
int ff_h264_svc_decode_slice_header(H264Context *h) {
	PPS *pps;
	SPS *sps;
	int num_ref_idx_active_override, slice_type, pps_id, chromaArrayType, first_mb_in_slice;
	u_int8_t base_pred_weight_table_flag;
	u_int8_t default_base_mode_flag;
	MpegEncContext * const s = &h->s;


	first_mb_in_slice = get_ue_golomb(&s->gb);

	slice_type = get_ue_golomb_31(&s->gb);
	if (slice_type > 9) {
		av_log(h->s.avctx, AV_LOG_ERROR, "slice type too large (%d) at %d %d\n",
				h->slice_type, s->mb_x, s->mb_y);
		return -1;
	}
	if (slice_type > 4) {
		slice_type -= 5;
		h->slice_type_fixed = 1;
	} else
		h->slice_type_fixed = 0;

	slice_type = golomb_to_pict_type[slice_type];

	h->slice_type = slice_type;
	h->slice_type_nos = slice_type & 3;

	pps_id = get_ue_golomb(&s->gb);

	// activate SPS and PPS in H264Context
	pps = get_PPS(h, h,pps_id, 1);
	sps = get_SPS(h, h ,h->pps.sps_id, 1);
	// set previous parsed values
	sps->pps_id = pps_id;
	sps->first_mb_in_slice = first_mb_in_slice;


	chromaArrayType = ff_h264_chroma_array_type(h, sps);

	if (sps->residual_color_transform_flag == 1) {
		get_bits(&s->gb, 2); /* colour_plane_id */
	}


	h->frame_num = get_bits(&s->gb, sps->log2_max_frame_num);
	if (!sps->frame_mbs_only_flag) {
		sps->field_pic_flag = get_bits1(&s->gb);
		if (sps->field_pic_flag) {
			sps->bottom_field_flag = get_bits1(&s->gb);
		}
	}
	if (h->idr_flag == 1) {
		h->idr_pic_id = get_ue_golomb(&s->gb);
	}
	if (sps->poc_type == 0) {
		h->poc_lsb = get_bits(&s->gb,sps->log2_max_poc_lsb);
		if (pps->pic_order_present && !sps->field_pic_flag) {
			h->delta_poc_bottom = get_se_golomb(&s->gb);
		}
	}
	if (sps->poc_type == 1 && !sps->delta_pic_order_always_zero_flag) {
		h->delta_poc[0] = get_se_golomb(&s->gb);
		if (pps->pic_order_present && !sps->field_pic_flag) {
			h->delta_poc[1] = get_se_golomb(&s->gb);
		}
	}
	if (pps->redundant_pic_cnt_present) {
		h->redundant_pic_count = get_ue_golomb(&s->gb);
	}
	if (h->quality_id == 0) {
		if (h->slice_type == AV_PICTURE_TYPE_EP) {
			h->direct_spatial_mv_pred = get_bits1(&s->gb);
		}
		if ( h->slice_type == AV_PICTURE_TYPE_EP
			|| h->slice_type == AV_PICTURE_TYPE_EB ) {
			num_ref_idx_active_override = get_bits1(&s->gb);
			if (num_ref_idx_active_override) {
				h->ref_count[0] = get_ue_golomb(&s->gb) + 1;
				if (h->slice_type == AV_PICTURE_TYPE_EB){
					h->ref_count[1] = get_ue_golomb(&s->gb) + 1;
				}

			}
		}
		ff_h264_decode_ref_pic_list_reordering(h);
		if ((pps->weighted_pred && h->slice_type == AV_PICTURE_TYPE_EP)
				|| (pps->weighted_bipred_idc == 1 &&  h->slice_type == AV_PICTURE_TYPE_EB)) {
			if (!h->no_inter_layer_pred_flag) {
				base_pred_weight_table_flag = get_bits1(&s->gb);
			}
			if (h->no_inter_layer_pred_flag || !base_pred_weight_table_flag) {
				pred_weight_table(h); //in h264.c
			}
		}
		if (h->nal_ref_idc != 0) {
			ff_h264_decode_ref_pic_marking(h, &s->gb);
			if (!sps->slice_header_restriction_flag) {
				h->store_ref_base_pic_flag = get_bits1(&s->gb);
				if ((h->use_ref_base_pic_flag || h->store_ref_base_pic_flag)
						&& !h->idr_flag) {
					//dec_ref_base_pic_marking();
					ff_h264_decode_ref_base_pic_marking(h);
				}

			}
		}
	}

	if (pps->cabac &&  h->slice_type != AV_PICTURE_TYPE_EI) {

		h->cabac_init_idc = get_ue_golomb(&s->gb);
	}

	s->qscale = pps->init_qp + get_se_golomb(&s->gb); /* slice_qp_delta */

	if (pps->deblocking_filter_parameters_present) {
		h->deblocking_filter = get_ue_golomb(&s->gb);
		if (h->deblocking_filter != 1) {
			h->slice_alpha_c0_offset += get_se_golomb(&s->gb) << 1;
			h->slice_beta_offset += get_se_golomb(&s->gb) << 1;
		}
	}


	if (pps->slice_group_count > 1 && pps->mb_slice_group_map_type >= 3
			&& pps->mb_slice_group_map_type <= 5) {
		// Not yet supported, slice_group_change_rate_minus1 missing
		/* pps->slice_group_change_cycle =  get_bits(&s->gb,ceil(log2((sps->mb_width * sps->mb_height)/pps->slice_group_change_rate + 1 ) )); */
	}

	if (!h->no_inter_layer_pred_flag && h->quality_id == 0) {
		get_ue_golomb(&s->gb); /* ref_layer_dq_id */
		if (sps->inter_layer_deblocking_filter_control_present_flag) {
			if (get_ue_golomb(&s->gb) != 1) { /* disable_inter_layer_deblocking_filter_idc */
				 get_se_golomb(&s->gb); /* inter_layer_slice_alpha_c0_offset_div2 */
				 get_se_golomb(&s->gb); /* inter_layer_slice_beta_offset_div2 */
			}
		}

		get_bits1(&s->gb); /* constrained_intra_resampling_flag */
		if (sps->extended_spatial_scalability_idc == 2) {
			if (chromaArrayType > 0) {
				 get_bits1(&s->gb); /* ref_layer_chroma_phase_x_plus1_flag */
				 get_bits(&s->gb, 2); /* ref_layer_chroma_phase_y_plus1 */
			}
			get_se_golomb(&s->gb); /* scaled_ref_layer_left_offset */
			get_se_golomb(&s->gb); /* scaled_ref_layer_top_offset */
			get_se_golomb(&s->gb); /* scaled_ref_layer_right_offset */
			get_se_golomb(&s->gb); /* scaled_ref_layer_bottom_offset */
		}
	}
	if (!h->no_inter_layer_pred_flag) {
		h->slice_skip_flag = get_bits1(&s->gb);
		if (h->slice_skip_flag)
			get_ue_golomb(&s->gb); /* num_mbs_in_slice_minus1 */
		else {

			if (!get_bits1(&s->gb)){ /* adaptive_base_mode_flag */
				default_base_mode_flag = get_bits1(&s->gb);
			}
			if (!default_base_mode_flag) {
				if (!get_bits1(&s->gb)){ /* adaptive_motion_prediction_flag*/
					 get_bits1(&s->gb); /* default_motion_prediction_flag */
				}
			}
			if (!get_bits1(&s->gb)){ /* adaptive_residual_prediction_flag */
				get_bits1(&s->gb); /* default_residual_prediction_flag */
			}
		}
		if (sps->adaptive_tcoeff_level_prediction_flag){
			get_bits1(&s->gb); /* tcoeff_level_prediction_flag */
		}
	}
	if (!sps->slice_header_restriction_flag && !h->slice_skip_flag) {
		get_bits(&s->gb, 4); /* scan_idx_start */
		get_bits(&s->gb, 4); /* scan_idx_end */
	}
	return 0;
}

/** G.7.3.3.5 */
int ff_h264_decode_ref_base_pic_marking(H264Context *h){
    MpegEncContext * const s = &h->s;
    int i;

    h->mmco_index= 0;
	if(get_bits1(&s->gb)){ // adaptive_ref_base_pic_marking_mode_flag
		for(i= 0; i<MAX_MMCO_COUNT; i++) {
			MMCOOpcode opcode = get_ue_golomb_31(&s->gb);

			h->mmco[i].opcode= opcode;
			if(opcode==MMCO_SHORT2UNUSED){
				h->difference_of_base_pic_nums_minus1 = get_ue_golomb(&s->gb);
				// h->mmco[i].short_pic_num= (h->curr_pic_num - h->difference_of_base_pic_nums_minus1 - 1) & (h->max_pic_num - 1);
			}
			if( opcode==MMCO_LONG2UNUSED){
				h->long_term_base_pic_num = get_ue_golomb(&s->gb);
				// h->mmco[i].long_arg= h->long_term_base_pic_num;
			}

			if(opcode > (unsigned)MMCO_LONG){
				av_log(h->s.avctx, AV_LOG_ERROR, "illegal memory management control operation %d\n", opcode);
				return -1;
			}
			if(opcode == MMCO_END)
				break;
		}
		h->mmco_index= i;
	}
    return 0;
}

/** H.7.3.1.1 */
void ff_h264_mvc_decode_nal_header(H264Context *h, const uint8_t *src) {
	//  +-------------------------------------------+---------------+--------------------------------------------------------------+
	//  |                 FIRST BYTE                |  SECOND BYTE  |                   THIRD BYTE                                 |
	//  +------------------+------------+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---+---+---+---------------+-------------+----------------+
	//  |         7        |      6     |5|4|3|2|1|0|7|6|5|4|3|2|1|0|7|6| 5 | 4 | 3 |       2       |      1      |        0       |
	//  +------------------+------------+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---+---+---+---------------+-------------+----------------+
	//  |SVC EXTENSION FLAG|NON IDR FLAG|PRIORITY ID|      VIEW ID      |TEMPORAL ID|ANCHOR PIC FLAG|INTER VIEW ID|RESERVED ONE BIT|
	//  +------------------+------------+-----------+-------------------+-----------+---------------+-------------+----------------+

	if(h->nal_unit_type == NAL_PREFIX || h->nal_unit_type == NAL_EXT_SLICE){

		h->non_idr_flag = ((src[0] & 0x40) >> 6);
		h->non_idr_present = 1;
		h->priority_id = (src[0] & 0x3F);
		h->view_id = (src[1] << 2) + ((src[2] & 0xC0) >> 6);
		//av_log(h->s.avctx, AV_LOG_ERROR, "ff_h264_mvc_decode_nal_header() src bytes =  %d : %d : %d\n", src[0],src[1],src[2]);
		//av_log(h->s.avctx, AV_LOG_ERROR, "ff_h264_mvc_decode_nal_header() view_id =  %d\n", h->view_id);
		h->temporal_id = (src[2] & 0x38) >> 3;
		h->anchor_pic_flag = (src[2] & 0x4) >> 2;
		h->inter_view_flag = (src[2] & 0x2) >> 1;
		h->is_mvc = 1;
		h->s.avctx->hwaccel = 0;
		//h->reserved_one_bit = src[2] & 0x1;
		//	av_log(h->s.avctx, AV_LOG_DEBUG,
		//			"MVC extension NAL unit with type: %d recieved \n",
		//			h->nal_unit_type);
	}
}




/** H.7.3.2.1.4 */
void ff_h264_mvc_decode_sps(H264Context *h, SPS *sps) {
	MpegEncContext * const s = &h->s;

	int i, j, k, tmp = 0;
	int num_ops_i, num_ops_i_j = 0;

	sps->num_views_minus1 = get_ue_golomb(&s->gb);
	for (i = 0; i <= sps->num_views_minus1; i++) {
		tmp = get_ue_golomb(&s->gb);
		sps->view_id[i] = tmp;
		//av_log(h->s.avctx, AV_LOG_DEBUG, "view_id[%d] =  %d \n", i, tmp);
	}
	for (i = 1; i <= sps->num_views_minus1; i++) {
		tmp = get_ue_golomb(&s->gb);
		sps->num_anchor_refs[0][i] = tmp;
		//av_log(h->s.avctx, AV_LOG_DEBUG, "num_anchor_refs_lX[0][%d] =  %d \n", i, tmp);
		for (j = 0; j < sps->num_anchor_refs[0][i]; j++) {
			sps->anchor_ref[0][i][j] = get_ue_golomb(&s->gb);
			//av_log(h->s.avctx, AV_LOG_DEBUG, "anchor_refs_lX[0][%d][%d] =  %d \n", i, j,sps->anchor_ref_lX[0][i][j]);
		}

		tmp = get_ue_golomb(&s->gb);
		sps->num_anchor_refs[1][i] = tmp;
		//av_log(h->s.avctx, AV_LOG_DEBUG, "num_anchor_refs_lX[1][%d] =  %d \n", i, tmp);
		for (j = 0; j < sps->num_anchor_refs[1][i]; j++) {
			sps->anchor_ref[1][i][j] = get_ue_golomb(&s->gb);
			//av_log(h->s.avctx, AV_LOG_DEBUG, "anchor_refs_lX[1][%d][%d] =  %d \n", i, j,sps->anchor_ref_lX[1][i][j]);
		}
	}
	for (i = 1; i <= sps->num_views_minus1; i++) {
		tmp = get_ue_golomb(&s->gb);
		sps->num_non_anchor_refs_lX[0][i] = tmp;
		//av_log(h->s.avctx, AV_LOG_DEBUG, "num_non_anchor_refs_lX[0][%d] =  %d \n", i, tmp);
		for (j = 0; j < sps->num_non_anchor_refs_lX[0][i]; j++) {
			sps->non_anchor_ref_lX[0][i][j] = get_ue_golomb(&s->gb);
			//av_log(h->s.avctx, AV_LOG_DEBUG, "non_anchor_ref_lX[0][%d][%d] =  %d \n", i, j,sps->non_anchor_ref_lX[0][i][j]);
		}

		tmp = get_ue_golomb(&s->gb);
		sps->num_non_anchor_refs_lX[1][i] = tmp;
		//av_log(h->s.avctx, AV_LOG_DEBUG, "num_non_anchor_refs_lX[1][%d] =  %d \n", i, tmp);
		for (j = 0; j < sps->num_non_anchor_refs_lX[1][i]; j++) {
			sps->non_anchor_ref_lX[1][i][j] = get_ue_golomb(&s->gb);
			//av_log(h->s.avctx, AV_LOG_DEBUG, "non_anchor_ref_lX[1][%d][%d] =  %d \n", i, j,sps->non_anchor_ref_lX[1][i][j]);
		}
	}
	sps->num_level_values_signalled_minus1 = get_ue_golomb(&s->gb);
	for (i = 0; i <= sps->num_level_values_signalled_minus1; i++) {
		get_bits(&s->gb, 8); //sps->level_idc_array[i] = get_bits(&s->gb, 8);
		num_ops_i = get_ue_golomb(&s->gb); //sps->num_applicable_ops_minus1[i] = get_ue_golomb(&s->gb);

		//for (j = 0; j <= sps->num_applicable_ops_minus1[i]; j++) {
		for (j = 0; j <= num_ops_i; j++) {

			get_bits(&s->gb, 3); //sps->applicable_op_temporal_id[i][j] = get_bits(&s->gb, 3);
			num_ops_i_j  = get_ue_golomb(&s->gb); //sps->applicable_op_num_target_views_minus1[i][j] = get_ue_golomb(&s->gb);

			for (k = 0; k <= num_ops_i_j;k++) {
				get_ue_golomb(&s->gb); //sps->applicable_op_target_view_id[i][j][k] = get_ue_golomb(&s->gb);
			}
			get_ue_golomb(&s->gb); //sps->applicable_op_num_views_minus1[i][j] = get_ue_golomb(&s->gb);
		}
	}
}

/** H.7.3.3.1.1 */
int ff_h264_mvc_reorder_ref_pic_list(H264Context *h, SPS* sps){
	MpegEncContext * const s = &h->s;
	int list, index, pic_structure;
	H264Context *h_main = h->mvc_context[0];
	if(!h_main){
		h_main = h;
	}
	//av_log(h->s.avctx, AV_LOG_INFO, "NAL unit type is %d\n", h->nal_unit_type);
	//av_log(h->s.avctx, AV_LOG_INFO, "NAL unit type is %d\n", );

	print_short_term(h);
	print_long_term(h);

	// av_log(h->s.avctx, AV_LOG_ERROR, "List count for view %d is %d.\n" , h->view_id, h->list_count);
	// h->list_count is 1 for I,SI slices and 2 for B slices
	for (list = 0; list < h->list_count; list++) {
		memcpy(h->ref_list[list], h->default_ref_list[list],
				sizeof(Picture) * h->ref_count[list]);
		if(h->anchor_pic_flag){
			int i, voidx;
			int ref_count = h->ref_count[list];
		//	av_log(h->s.avctx, AV_LOG_DEBUG,"reference count is %d\n", ref_count);
			for(index = 0; index < ref_count; index++){
				if (!h->ref_list[list][index].f.data[0]) {
					break;
				}
			}
			voidx = ff_h264_mvc_get_voidx(h, &h->sps);
			for(i = 0; i < h->sps.num_anchor_refs[list][voidx]; i++){
				if(index >= ref_count ){
					break;
				}
				if(h_main->inter_view_ref_list[h->sps.anchor_ref[list][h->view_id][i]]){
					h->ref_list[list][index++] = *h_main->inter_view_ref_list[h->sps.anchor_ref[list][h->view_id][i]];
				}
			}

		}

		if (get_bits1(&s->gb)) { //ref_pic_list_modification_flag_lX, X=list
			int pred = h->curr_pic_num; /* picNumLXPred  X=list */
			int view_idx_pred = -1; /* picViewIdxLXPred */

			for (index = 0;; index++) { /* refIdxLX, X=list */
				unsigned int modification_of_pic_nums_idc = get_ue_golomb_31(&s->gb);
				unsigned int pic_id,
				modification_type = 0; // 1 for short, 2 for long and 3 for mvc
				int i = 0;
				Picture *ref = NULL;
				//av_log(h->s.avctx, AV_LOG_DEBUG, "reference %d\n",i);

		        if(modification_of_pic_nums_idc==3)
		            break;

				if (index >= h->ref_count[list]) {
					av_log(h->s.avctx, AV_LOG_ERROR,"reference count overflow\n");
					return -1;
				}

				if (modification_of_pic_nums_idc < 6) {
					if (modification_of_pic_nums_idc < 2) {
						/* modification_of_pic_nums_idc ==  0 || modification_of_pic_nums_idc == 1
						 * H.8.2.2.1 applies */
						const unsigned int abs_diff_pic_num = get_ue_golomb(
								&s->gb) + 1; /* abs_diff_pic_num_minus1 + 1 */
						int frame_num;
						modification_type = 1;
						if (abs_diff_pic_num > h->max_pic_num) {
							av_log(h->s.avctx, AV_LOG_ERROR,
									"abs_diff_pic_num overflow\n");
							return -1;
						}

						if (modification_of_pic_nums_idc == 0)
							pred -= abs_diff_pic_num;
						else
							pred += abs_diff_pic_num;
						/* maxPicNum = [2 *] 2^(log2_max_frame_num_minus4 + 4)
						 * 		=> mask = max_pic_num -1 = 011111....1 bit mask
						 * 		=> pred & mask = pred wrapped to it's range:
						 *
						 * 		pred = (pred < 0 ) ? pred + h->max_pic_num : pred;
						 * 		pred = (pred > h->max_pic_num) ? pred - h->max_pic_num : pred;
						 */
						pred &= h->max_pic_num - 1; /* pred = picNumLXNoWrap  X=list */

						frame_num = pic_num_extract(h, pred, &pic_structure);

						for (i = h->short_ref_count - 1; i >= 0; i--) {
							ref = h->short_ref[i];
							assert(ref->f.reference);
							assert(!ref->long_ref);
							if (ref->frame_num == frame_num
									&& (ref->f.reference & pic_structure) && ref->view_id == h->view_id)
								break;
						}
						if (i >= 0)
							ref->pic_id = pred;
					} else if (modification_of_pic_nums_idc == 2){
						/* modification_of_pic_nums_idc ==  2
						 * H.8.2.2.2 applies */
						int long_idx;
						pic_id = get_ue_golomb(&s->gb); //long_term_pic_idx

						long_idx = pic_num_extract(h, pic_id, &pic_structure);
						modification_type = 2;
						if (long_idx > 31) {
							av_log(h->s.avctx, AV_LOG_ERROR,
									"long_term_pic_idx overflow\n");
							return -1;
						}
						ref = h->long_ref[long_idx];
						assert(!(ref && !ref->f.reference));
						if (ref && (ref->f.reference & pic_structure)) {
							ref->pic_id = pic_id;
							assert(ref->long_ref);
							i = 0;
						} else {
							i = -1;
						}
					}else{
						/* modification_of_pic_nums_idc ==  4 || modification_of_pic_nums_idc == 5
						 * H.8.2.2.3 applies */
						unsigned int abs_diff_view_idx;
						int maxViewIdx, targetViewID;
						int *view_ref_list;

						int currVOIdx = ff_h264_mvc_get_voidx(h, sps);

						//av_log(h->s.avctx, AV_LOG_DEBUG,"modify ref_list %d of view_id %d with vOIdx %d\n", list, h->view_id, currVOIdx);

						abs_diff_view_idx = get_ue_golomb(&s->gb) + 1;
						modification_type = 3;

						if(h->anchor_pic_flag) {
							maxViewIdx = sps->num_anchor_refs[list][currVOIdx]-1;
							view_ref_list = sps->anchor_ref[list][currVOIdx];
						} else {
							maxViewIdx = sps->num_non_anchor_refs_lX[list][currVOIdx]-1;
							view_ref_list = sps->non_anchor_ref_lX[list][currVOIdx];
						}
						if((((int) abs_diff_view_idx) - 1) < 0
						   || (abs_diff_view_idx - 1) > max(0,maxViewIdx)){
							av_log(h->s.avctx, AV_LOG_ERROR,"abs_diff_view_idx_minus1 exceeds range\n");
							return -1;
						}

						if (modification_of_pic_nums_idc == 4) {
							view_idx_pred -= abs_diff_view_idx;
						} else {
							view_idx_pred += abs_diff_view_idx;
						}
						if(view_idx_pred < 0 ){
							view_idx_pred  += maxViewIdx + 1;
						}else if( view_idx_pred > maxViewIdx){
							view_idx_pred  -= maxViewIdx + 1;
						}

						targetViewID = view_ref_list[view_idx_pred];

						if(h_main->inter_view_ref_list[targetViewID]){
							ref = h_main->inter_view_ref_list[targetViewID];
							ref->pic_id = h->curr_pic_num;
						}


					}

					if (i < 0) {
						av_log(h->s.avctx, AV_LOG_ERROR,
								"reference picture missing during reorder\n");
						memset(&h->ref_list[list][index], 0, sizeof(Picture)); //FIXME
					} else {
						int nIdx = index+1;

			 			for(i=index; i+1<h->ref_count[list]; i++){
                            if(ref->long_ref == h->ref_list[list][i].long_ref && ref->pic_id == h->ref_list[list][i].pic_id)
                                break;
                        }
						//for(i = h->ref_count[list]; i > index; i--){ /* for( cIdx = num_ref_idx_lX_active_minus1 + 1; cIdx > refIdxLX; cIdx−− ) */
   						for(; i > index; i--){
							h->ref_list[list][i]= h->ref_list[list][i-1];
						}
   						//av_log(h->s.avctx, AV_LOG_DEBUG,"assign h->ref_list_mvc[view_id=%d][list=%d][index=%d] to reference {pic_id=%d, frame_num=%d, view_id=%d}\n",h->view_id,list,index, ref->pic_id, ref->frame_num, ref->view_id);
   						h->ref_list[list][index] = *ref;
						if (FIELD_PICTURE){
							pic_as_field(&h->ref_list[list][index],pic_structure);
						}
						for( i = index+1; i <= h->ref_count[list]; i++ ){
							if(	   ((h->ref_list[list][i].pic_id   != ref->pic_id) && (modification_type == 1 || modification_type == 3) )
								|| (h->ref_list[list][i].view_id  != ref->pic_id && modification_type == 3)
								|| (h->ref_list[list][i].long_ref != ref->long_ref  && modification_type == 2)){
								h->ref_list[list][nIdx++] = h->ref_list[list][i];
							}
						}

						/**
						 * SHORT_TERM: (8.2.4.3.1)
						 nIdx = refIdxLX
						 for( cIdx = refIdxLX; cIdx <= num_ref_idx_lX_active_minus1 + 1; cIdx++ ) (8-37)
							if( PicNumF( RefPicListX[ cIdx ] ) != picNumLX )
								RefPicListX[ nIdx++ ] = RefPicListX[ cIdx ]

						 * LONG_TERM: (8.2.4.3.2)
						 nIdx = refIdxLX
						 for( cIdx = refIdxLX; cIdx <= num_ref_idx_lX_active_minus1 + 1; cIdx++ ) (8-38)
							if( LongTermPicNumF( RefPicListX[ cIdx ] ) != long_term_pic_num )
								RefPicListX[ nIdx++ ] = RefPicListX[ cIdx ]

						 * MVC: (H.8.2.2.1)
						 nIdx = refIdxLX
						 for( cIdx = refIdxLX; cIdx <= num_ref_idx_lX_active_minus1 + 1; cIdx++ ) (H-4)
							if( PicNumF( RefPicListX[ cIdx ] ) != picNumLX | | viewID(RefPicListX[ cIdx ] ) != currViewID )
								RefPicListX[ nIdx++ ] = RefPicListX[ cIdx ]

						 *
						 */

					}
				} else {
					av_log(h->s.avctx, AV_LOG_ERROR,
							"illegal modification_of_pic_nums_idc\n");
					return -1;
				}
			}
		}
	}

    for(list=0; list<h->list_count; list++){
        for(index= 0; index < h->ref_count[list]; index++){
            if (!h->ref_list[list][index].f.data[0]) {
                av_log(h->s.avctx, AV_LOG_ERROR, "Missing reference picture (%d)\n", index);
                if (h->default_ref_list[list][0].f.data[0]){
                    h->ref_list[list][index]= h->default_ref_list[list][0];
                }
                else{
                    return -1;
                }
            }else{
            	//av_log(h->s.avctx, AV_LOG_DEBUG, "refPicList%d[%d] :  Picture( pic_id:%d, long_ref:%d, view_id:%d)\n",list,index, h->ref_list[list][index].pic_id, h->ref_list[list][index].long_ref, h->ref_list[list][index].view_id);
            }
        }
    }

    return 0;
}

/** H.7.4.1.1 */
int ff_h264_deploy_prefix_nal_threaded(H264Context *h, H264Context *h0) {
	int err = 0;
	if(h->nal_unit_type != NAL_PREFIX){
		if (h0->prefix_nal_present != 0) {
			// reset prefix flag
			h0->prefix_nal_present = 0;

			// do some bit stream validation and error parsing
			if(h->nal_ref_idc != h0->nal_ref_idc){
				// nal_ref_idc has to be equal for prefix and it's associated NAL unit.
				av_log(h->s.avctx, AV_LOG_ERROR, "Value mismatch: nal_ref_idc of prefix NAL unit and current NAL unit shall be equal to each other.\n");
				err -= 1;
			}

			// copy values from prefix nal unit to current one.
			h->non_idr_flag  = h0->non_idr_flag,
			h->non_idr_present = h0->non_idr_present;
			h->priority_id = h0->priority_id;
			h->view_id = h0->view_id;
			h->temporal_id = h0->temporal_id;
			h->anchor_pic_flag = h0->anchor_pic_flag;
			h->inter_view_flag = h0->inter_view_flag;

		}else{
		// if no prefix NAL unit is present, infer the missing values
			if(h->nal_unit_type == NAL_SLICE || h->nal_unit_type == NAL_IDR_SLICE) {
				h->temporal_id = h0->temporal_id;
				h->anchor_pic_flag = h0->anchor_pic_flag;
			}

		}

	}
	err += ff_h264_mvc_deploy_nal_header(h);
	return err;
}

int ff_h264_mvc_deploy_nal_header(H264Context *h) {
	int err = 0;
	if(  h->is_mvc && (h->nal_unit_type == NAL_SLICE
			|| h->nal_unit_type == NAL_IDR_SLICE
			|| h->nal_unit_type == NAL_DPA
			|| h->nal_unit_type == NAL_AUXILIARY_SLICE) ){
		if (h->prefix_nal_present != 0) {
			// reset prefix flag
			h->prefix_nal_present = 0;

			if(h->non_idr_present && h->non_idr_flag == 0 && h->nal_unit_type != NAL_IDR_SLICE){
				// if non_idr_flag is 0 in prefix NAL unit,
				// the associated NAL unit shall have NAL unit type equal to 5.
				av_log(h->s.avctx, AV_LOG_ERROR, "Value mismatch: When non_idr_flag is equal to 0, the value of nal_unit_type shall be equal to 5.\n");
				err -= 1;
			}
			if(h->non_idr_flag == 1 && h->nal_unit_type != NAL_SLICE){
				// if non_idr_flag is 1 in prefix NAL unit,
				// the associated NAL unit shall have NAL unit type equal to 1.
				av_log(h->s.avctx, AV_LOG_ERROR, "Value mismatch: When non_idr_flag is equal to 1, the value of nal_unit_type shall be equal to 1.\n");
				err -= 1;
			}
		}else{
			// if no prefix NAL unit is present, infer the missing values
			h->anchor_pic_flag = 0;
			h->non_idr_present = 1;
			h->priority_id = 0;
			h->view_id = 0;
			h->inter_view_flag =1;

			if(h->nal_unit_type == NAL_IDR_SLICE) {
				h->non_idr_flag = 0;
			}else{
				h->non_idr_flag = 1;
			}

		}
		// transfer non_idr_flag to idr_flag
		if(h->non_idr_present){
			h->idr_flag = !h->non_idr_flag;
		}else{
			// if not rule applies, assume no IDR picture is present.
			h->non_idr_flag = 1;
		}

		// do some bit stream validation and error parsing
		if(h->nal_ref_idc == 0 && h->non_idr_flag == 0){
			av_log(h->s.avctx, AV_LOG_ERROR, "Value mismatch: When nal_ref_idc is equal to 0, the value of non_idr_flag shall be equal to 1.\n");
			err -= 1;
		}
		if( h->non_idr_flag == 0 && h->anchor_pic_flag == 0){
			av_log(h->s.avctx, AV_LOG_ERROR, "Value mismatch: When non_idr_flag is equal to 0, anchor_pic_flag shall be equal to 1.\n");
			err -= 1;
		}
		if( h->nal_ref_idc == 0 && h->anchor_pic_flag == 1){
			av_log(h->s.avctx, AV_LOG_ERROR, "Value mismatch: When nal_ref_idc is equal to 0, the value of anchor_pic_flag shall be equal to 0.\n");
			err -= 1;
		}
	}
	return err;
}


/** H.7.4.1.1 */
int ff_h264_mvc_get_voidx(H264Context *h, SPS *sps) {
	int view_id = h->view_id;
	int i = sps->id;
	SPS* sub_sps = 0;
	H264Context *h_main = h->mvc_context[0];
	if(!h_main){
		h_main = h;
	}
	if(!sps->is_sub_sps){
		if(h_main->sub_sps_buffers[i]){
			sub_sps = h_main->sub_sps_buffers[i];
		}
		else{
			for(i=0; i< MAX_SPS_COUNT; i++){
				if(h_main->sub_sps_buffers[i]){
					sub_sps = h_main->sub_sps_buffers[i];
					break;
				}
			}
		}
		if(sub_sps){
			av_log(h->s.avctx, AV_LOG_ERROR, "Wrong SPS (id=%d), sub-SPS needed. Took sub-SPS (id=%d) instead.\n",sps->id,sub_sps->id);
		}else{
			av_log(h->s.avctx, AV_LOG_ERROR, "Wrong SPS (id=%d), sub-SPS needed but not found.\n",sps->id);
			return -1;
		}
	}else{
		sub_sps = sps;
	}

	if (sub_sps->view_id[h->view_id] > sub_sps->num_views_minus1) {
		av_log(h->s.avctx, AV_LOG_ERROR,
				"sps->view_id overflow, num_views_minus1 to big\n");
		return 0;
	}

	for (i = 0; i <= sub_sps->num_views_minus1; i++) {
		if (sub_sps->view_id[i] == view_id) {
			return i;	//VOIdx
		}
	}

	// VOIdx not found
	av_log(h->s.avctx, AV_LOG_ERROR, "VOIdx not found for view %d\n",view_id);
	return 0;
}

/** H.10.2.1 */
void ff_h264_mvc_realloc_dpb(H264Context *h) {
	MpegEncContext *s = &(h->s);
	int i, old_picture_count, new_picture_count;
	if(!s->mvc_dbp_initialized) {
		old_picture_count = s->picture_count;
		new_picture_count =
				FFMAX(MAX_PICTURE_COUNT, MAX_PICTURE_COUNT*ceil(log2(MAX_VIEW_COUNT)))
				* FFMAX(1, s->avctx->thread_count);
		if (old_picture_count == new_picture_count) {
			return;
		} else {
			s->picture_count = new_picture_count;
			av_realloc(s->picture, s->picture_count * sizeof(Picture));
			for (i = old_picture_count; i < new_picture_count; i++) {
				avcodec_get_frame_defaults(&s->picture[i].f);
			}
			av_log(h->s.avctx, AV_LOG_INFO, "reallocate DPB memory for h264 MVC from %d to %d \n", old_picture_count, new_picture_count);
			s->mvc_dbp_initialized = 1;
		}
	}
}

/* H.10.2.1 */
void ff_h264_init_picture_count(H264Context *h, MpegEncContext *s){
	int i;
	H264Context *h_main = h->mvc_context[0];
	if(!h_main){
		h_main = h;
	}
	if(h->is_mvc && !s->mvc_dbp_initialized) {
		SPS* sps = &h->sps;
		if(!sps->is_sub_sps){
			for(i=0; i< MAX_SPS_COUNT; i++){
				if(h_main->sub_sps_buffers[i]){
					sps = h_main->sub_sps_buffers[i];
					break;
				}
			}
		}

		s->max_picture_count = FFMAX(MAX_PICTURE_COUNT, MAX_PICTURE_COUNT*ceil(log2(sps->num_views_minus1 + 1)))
				* FFMAX(1, s->avctx->thread_count);
		if(s->picture_range_end - s->picture_range_start < s->max_picture_count){
			s->picture_range_start = (s->picture_range_start/MAX_PICTURE_COUNT)*s->max_picture_count;
			s->picture_range_end = (s->picture_range_end/MAX_PICTURE_COUNT)*s->max_picture_count;
		}
		s->mvc_dbp_initialized = 1;
	}
}

/** H.14.1 */
int ff_h264_mvc_decode_vui_parameters(H264Context *h, SPS *sps) {
	MpegEncContext * const s = &h->s;

	int i, j;
	u_int8_t vui_mvc_timing_info_present_flag;
	u_int8_t vui_mvc_nal_hrd_parameters_present_flag;
	u_int8_t vui_mvc_vcl_hrd_parameters_present_flag;

	sps->inter_layer_deblocking_filter_control_present_flag = get_bits1(&s->gb);
	sps->vui_mvc_num_ops_minus1 = get_ue_golomb(&s->gb);
	for (i = 0; i <= sps->vui_mvc_num_ops_minus1; i++) {
		sps->vui_mvc_temporal_id[i] = get_bits(&s->gb, 3);
		sps->vui_mvc_num_target_output_views_minus1[i] = get_ue_golomb(&s->gb);
		for (j = 0; j <= sps->vui_mvc_num_target_output_views_minus1[i]; j++) {
			sps->vui_mvc_view_id[i][j] = get_ue_golomb(&s->gb);
		}
		vui_mvc_timing_info_present_flag = get_bits1(&s->gb);
		if (vui_mvc_timing_info_present_flag) {
			sps->vui_mvc_num_units_in_tick[i] = get_bits(&s->gb, 32);
			sps->vui_mvc_time_scale[i] = get_bits(&s->gb, 32);
			sps->vui_mvc_fixed_frame_rate_flag[i] = get_bits1(&s->gb);
		}
		vui_mvc_nal_hrd_parameters_present_flag = get_bits1(&s->gb);
		vui_mvc_vcl_hrd_parameters_present_flag = get_bits1(&s->gb);
		if (vui_mvc_nal_hrd_parameters_present_flag
				|| vui_mvc_vcl_hrd_parameters_present_flag) {
			if (decode_hrd_parameters(h, sps) < 0)
				return -1;
			sps->vui_mvc_low_delay_hrd_flag[i] = get_bits1(&s->gb);
		}
		sps->vui_mvc_pic_struct_present_flag[i] = get_bits1(&s->gb);
	}
	return 0;
}
