/*
 * h264_mvc.h
 * Copyright (c) 2012 Jochen Britz
 */

/**
 * @file
 * H.264 / AVC / MPEG4 part10 codec /
 * SVC/MVC extension specific functions
 * @author: Jochen Britz
 */

#ifndef H264MVC_H_
#define H264MVC_H_

#include "h264.h"


// SVC slice types
enum SVCAVPictureType {
	AV_PICTURE_TYPE_EI = AV_PICTURE_TYPE_I,
	AV_PICTURE_TYPE_EP = AV_PICTURE_TYPE_P,
	AV_PICTURE_TYPE_EB = AV_PICTURE_TYPE_B
};

// ==================================================================== //
//  							HELPER									//
// ==================================================================== //


/**
 * A wrapper function for @see ff_thread_await_progress() for easier modification to adapt for MVC.
 *
 * @param[in] h The current main H264Context (typically, h0 or h).
 * @param[in] ref_pic The reference picture to wait for.
 * @param[in] n The number the @see ff_h264_thread_report_progress() should reach to stop waiting (typically, the current row).
 * @param[in] field The field to wait for (either 0 or 1).
 */
void ff_h264_thread_await_progress(H264Context *h, Picture *ref_pic, int n, int field);

/**
 * A wrapper function for @see ff_thread_report_progress() for easier modification to adapt for MVC.
 *
 * @param[in] h The current main H264Context (typically, h0 or h).
 * @param[in] ref_pic The reference picture to wait for.
 * @param[in] n The number to report (typically, the current row).
 * @param[in] field The field to report in (either 0 or 1).
 */
void ff_h264_thread_report_progress(H264Context *h, Picture *ref_pic, int n, int field);


/**
 * A function similar to @see ff_h264_thread_await_progress(), but waits for pictures from other views.
 * This is done through a extension of the field indexing to view indexing.
 *
 * @param[in] h The current main H264Context (typically, h0 or h).
 * @param[in] ref_pic The reference picture to wait for.
 */
void ff_h264_thread_await_picture(H264Context *h, Picture *ref_pic);


/**
 * A function similar to @see ff_h264_thread_report_progress(), but reports the
 * current pictures (@see h->s.current_picture_ptr) from the current view (@see h->s.current_picture_ptr->view_id).
 * This is done through a extension of the field indexing to view indexing.
 *
 * @param[in] h The current main H264Context (typically, h0 or h).
 */
void ff_h264_thread_report_Picture(H264Context *h);


/**
 * Parses the option string stored in @see h->target_view_indices;
 * These option can be set as command line parameter.
 *
 *
 * @param[in] h The context, which contains the option (typically, the main context of the base view).
 */
void parse_option_string(H264Context* h);

/**
 * Assigns H264Context* with view order index vOIdx to *h and returns MpegEncContext*.
 * Normal use case is:
 *			MpegEncContext * const s = ff_h264_extract_Context(avctx, &h, i);
 *
 * vOIdx has to be in the range 0..MAX_VIEW_COUNT otherwise vOIdx = 0 is taken.
 *
 * @param[in] avctx The AVCodecContext, which contains the H264Context in the priv_data field.
 * @param[out] h A poniter to the H264Context, corresponding to the vOIDx.
 * @param[in] voidx The View Order InDeX of the requested view (and corresponding H264Context).
 * @return MpegEncContext A pointer to the first field of the corresponding H264Context.
 */
MpegEncContext* ff_h264_extract_Context(const AVCodecContext * avctx, H264Context** h, int voidx);

/**
 * Assigns H264Context* with view order index vOIdx to *h and returns MpegEncContext*.
 * Similar to @see ff_h264_extract_Context() but for the parser.
 * Normal use case is:
 *			MpegEncContext * const s = ff_h264_extract_parser_Context(avctx, &h, i);
 *
 * vOIdx has to be in the range 0..MAX_VIEW_COUNT otherwise vOIdx = 0 is taken.
 *
 * @param[in] avctx The AVCodecParserContext, which contains the H264Context in the priv_data field.
 * @param[out] h A poniter to the H264Context, corresponding to the vOIDx.
 * @param[in] voidx The View Order InDeX of the requested view (and corresponding H264Context).
 * @return MpegEncContext A pointer to the first field of the corresponding H264Context.
 */
MpegEncContext* ff_h264_extract_parser_Context(const AVCodecParserContext* avctx, H264Context** h, int voidx);


/**
 * Returns a pointer to the MpegEncContext of the base-view.
 *
 * @param h The current main H264Context (typically, h0 or h).
 * @return A pointer to the MpegEncContext of the base-view.
 */
MpegEncContext* ff_h264_get_MpegEncContext(H264Context *h);

/**
 * Similar to the @see ff_h264_find_frame_end(). Searches for the end of a frame.
 * Takes MVC frames also into account.
 *
 * @param h The H264Context to work on.
 * @param buf	The buffer that stores the bit-stream
 * @param buf_size The size of the above buffer.
 * @return The position of the end of the frame in term of buffer index.
 */
int ff_h264_find_mvc_frame_end(H264Context *h, const uint8_t *buf, int buf_size);


/**
 * Initialize MVC-specific content in the H264Context.
 * At the moment is does nothing but is called in every initialization step,
 * to be able to quickly initialize new introduced fields.
 *
 * Is also used in  @see extract_H264Context().
 *
 * @param h The H264Context to initialize.
 */
void init_H264Context(H264Context *h);

/**
 * If a SPS with same id exists, the old SPS is deleted and replaced.
 * Depending on NAL unit type (NAL_SUB_SPS, NAL_SPS, NAL_SPS_EXT)
 * the parameter set is put to @see sub_sps_buffer or @see sps_buffer.
 *
 * If activate_it is set to 1, the SPS (or Subset SPS) is activated.
 *
 * @param h A pointer to the h264Context to put (and activate) the SPS. WARNING: this context have to be a main and not a threaded context!
 * @param sps A pointer to the SPS, which should be stored in buffer.
 * @param activate_it If set to 1, h->sps is overwritten by the given sps.
 * @return \li{Success, but existing SPS is overwritten: 1} \li{Success: 0} \li{Error: -1}
 */
int save_SPS(H264Context *h, SPS* sps, uint8_t activate_it);

/**
 * Depending on the current NAL unit type, the SPS is taken from the sps_buffer or the sub_sps_buffer.
 * If activate_it is set to 1, the SPS (or Subset SPS) is activated.
 *
 * @param h A pointer to the H264Context.
 * @param h0 A pointer to the main h264Context (in case of threading, typically h0. Otherwise put h to both).
 * @param sps_id The id of the required SPS or Subset SPS
 * @param activate_it If set to 1, h->sps is overwritten by the given sps.
 * @return \li{Success: the pointer to the activated SPS} \li{Error: 0}
 */
SPS* get_SPS(H264Context *h, H264Context *h0, uint sps_id, uint8_t activate_it);

/**
 *	Adds the PPS to the buffer of each MVC context.
 * 	If a PPS with same id exit, the old PPS is deleted and replaced.
 * 	If activate_it is set to 1, the PPS is activated.
 *
 * @param h A pointer to the H264Context. !WARNING: this context have to be a main and not a threaded context!
 * @param pps A pointer to the PPS, which should be stored in buffer.
 * @param pps_id The id of the PPS. Required, since the pps has no field for its id.
 * @param activate_it If set to 1, h->pps is overwritten by the given pps.
 * @return \li{Override: 1} \li{Success: 0} \li{Error: -1}
 */
int save_PPS(H264Context *h, PPS* pps, uint pps_id, uint8_t activate_it);

/**
 * Activates the PPS from pps_buffer and the SPS with id = pps.sps_id.
 * For SPS activation @see activate_SPS() is used.
 *
 * @param h A pointer to the H264Context.
 * @param h0 A pointer to the main h264Context (in case of threading, typically h0. Otherwise put h to both).
 * @param pps_id The id of the required PPS.
 * @param activate_it
 * @return PPS*  \li{Success: the pointer to the activated PPS} \li{Error: 0}
 */
PPS* get_PPS(H264Context *h, H264Context *h0, uint pps_id, uint8_t activate_it);

// ==================================================================== //
//  							CLAUSES									//
// ==================================================================== //

/** 7.3.2.1.2
 * 	Sequence parameter set extension RBSP syntax
 *
 * 	seq_parameter_set_extension_rbsp()
 */
void ff_h264_decode_sps_ext(H264Context *h, H264Context *h0);

/** 7.3.2.1.3
 * 	Subset sequence parameter set RBSP syntax
 *
 * 	subset_seq_parameter_set_rbsp()
 */
int ff_h264_decode_sub_sps(H264Context *h, int bit_length);

/** 7.3.2.10
 * 	RBSP slice trailing bits syntax
 *
 * 	rbsp_slice_trailing_bits()
 */
int ff_h264_decode_slice_trailing_bits(H264Context *h);

/** 7.3.2.11
 * 	RBSP trailing bits syntax
 *
 * 	rbsp_trailing_bits()
 */
int ff_h264_decode_trailing_bits(H264Context *h);

/** 7.3.2.12
 * 	Prefix NAL unit RBSP syntax
 *
 *	prefix_nal_unit_rbsp( )
 */
void ff_h264_decode_nal_prefix_threaded(H264Context *h, H264Context *h0, int bit_length);
void ff_h264_decode_nal_prefix(H264Context *h, int bit_length);

/** 7.3.2.13
 * 	Slice layer extension RBSP syntax
 *
 * 	slice_layer_extension_rbsp()
 */
void ff_h264_decode_slice_ext(H264Context *h);

/** 7.3.3
 * 	Slice header syntax
 *
 *	slice_header()
 *
 *	also available as inline function h264_ps.c with more initializing
 */
int ff_h264_decode_slice_header(H264Context *h);

/**	7.3.4
 * 	Slice data syntax
 *
 *	slice_data()
 */
void ff_h264_decode_slice_data(H264Context *h, SPS *sps);

/** 7.4.2.1.1
 * 	Sequence parameter set data semantics
 *
 * 	Calculates the ChromaArrayType
 *
 * 	"...
 *	Depending on the value of separate_colour_plane_flag,
 *	the value of the variable ChromaArrayType is assigned as follows:
 *		– If separate_colour_plane_flag is equal to 0, ChromaArrayType is set equal to chroma_format_idc.
 *		– Otherwise (separate_colour_plane_flag is equal to 1), ChromaArrayType is set equal to 0.
 *	..."
 */
int ff_h264_chroma_array_type(H264Context *h, SPS *sps);

// ==================================================================== //
//  							ANNEX G									//
// ==================================================================== //

/**	G.7.3.1.1
 * 	NAL unit header SVC extension syntax
 *
 *	nal_unit_header_svc_extension()
 */
void ff_h264_svc_decode_nal_header(H264Context *h);

/** G.7.3.2.1.4
 * 	Sequence parameter set SVC extension syntax
 *
 *	seq_parameter_set_svc_extension()
 */
void ff_h264_svc_decode_sps(H264Context *h, SPS *sps);

/** G.7.3.2.12.1
 * 	Prefix NAL unit SVC syntax
 *
 *	prefix_nal_unit_svc( )
 */
void ff_h264_svc_decode_nal_prefix(H264Context *h, int bit_length);

/** G.7.3.3.4
 * 	Slice header in scalable extension syntax
 *
 *  slice_header_in_scalable_extension()
 */
int ff_h264_svc_decode_slice_header(H264Context *h);
int ff_h264_svc_decode_slice_header_threaded(H264Context *h, H264Context *h0);

/** G.7.3.3.5
 *  Decoded reference base picture marking syntax
 *
 *  dec_ref_base_pic_marking()
 */
int ff_h264_decode_ref_base_pic_marking(H264Context *h);

// ==================================================================== //
//  							ANNEX G									//
// ==================================================================== //

/** H.7.3.1.1
 * 	NAL unit header MVC extension syntax
 *
 * 	nal_unit_header_mvc_extension()
 */
void ff_h264_mvc_decode_nal_header(H264Context *h, const uint8_t *src);

/** H.7.3.2.1.4
 * 	Sequence parameter set MVC extension syntax
 *
 * 	seq_parameter_set_mvc_extension()
 */
void ff_h264_mvc_decode_sps(H264Context *h, SPS *sps);

/** H.7.3.3.1.1
 * 	Reference picture list MVC modification syntax
 *
 *  ref_pic_list_mvc_modification()
 */
int ff_h264_mvc_reorder_ref_pic_list(H264Context *h, SPS* sps);
int ff_h264_build_default_inter_ref_list(H264Context* h, Picture *ref_list, int list, int len);
/** H.7.4.1.1
 *  NAL unit header MVC extension semantics
 *
 */
int ff_h264_deploy_prefix_nal_threaded(H264Context *h, H264Context *h0);
int ff_h264_mvc_deploy_nal_header(H264Context *h);

/** H.7.4.1.1
 * 	Returns the VOIdx of the view with id view_id.
 * 	If the VOIdx could not be calculated, 0 is returned
 * 	and the reason is logged as error.
 *
 * 	NAL unit header MVC extension semantics
 *
 * 	"...
 * 	The variable VOIdx, representing the view order index of the view identified by view_id, is set equal to the value of i for
 *  which the syntax element view_id[ i ] included in the referred subset sequence parameter set is equal to view_id.
 *	..."
 *
 */
int ff_h264_mvc_get_voidx(H264Context *h, SPS *sps);

/** H.10.2.1
 * 	Reallocates the DPB according to H.264 Annex H.
 *
 *  h->s.picture is found as DPB
 *
 * 	Level limits common to Multiview High and Stereo High profiles
 *
 * 	"...
 * 	MaxDpbFrames is equal to
 * 	Min( mvcScaleFactor * MaxDpbMbs / ( PicWidthInMbs * FrameHeightInMbs ),
 * 	Max( 1, Ceil( log2( NumViews ) ) ) * 16 ) and MaxDpbMbs is specified in Table A-1.
 * 	..."
 *
 */
void ff_h264_mvc_realloc_dpb(H264Context *h);

/** H.10.2.1
 * 	Initializes/Recalculates the picture count for DPB according to H.264 Annex H.
 *	In addition the corresponding fields s->picture_range_start and s->picture_range_end are adapted.
 *
 * 	Level limits common to Multiview High and Stereo High profiles
 * 	"...
 * 	MaxDpbFrames is equal to
 * 	Min( mvcScaleFactor * MaxDpbMbs / ( PicWidthInMbs * FrameHeightInMbs ),
 * 	Max( 1, Ceil( log2( NumViews ) ) ) * 16 ) and MaxDpbMbs is specified in Table A-1.
 * 	..."
 *
 */
void ff_h264_init_picture_count(H264Context *h, MpegEncContext *s);

/** H.14.1
 * 	MVC VUI parameters extension syntax
 *
 *  mvc_vui_parameters_extension()
 */
int ff_h264_mvc_decode_vui_parameters(H264Context *h, SPS *sps);

#endif /* H264MVC_H_ */
