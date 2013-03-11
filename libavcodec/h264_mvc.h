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

enum SVCAVPictureType {
    AV_PICTURE_TYPE_EI = AV_PICTURE_TYPE_I,
    AV_PICTURE_TYPE_EP = AV_PICTURE_TYPE_P,
    AV_PICTURE_TYPE_EB = AV_PICTURE_TYPE_B
};

// ==================================================================== //
//  							HELPER									//
// ==================================================================== //

/** MIN
 * 	Returns the minimum of two integer values
 */
int min(int a, int b);


/** MAX
 * 	Returns the maximum of two integer values
 */
int max(int a, int b);

/** EXTRACT
 * 	assigns H264Context* with view order index vOIdx to *h and returns MpegEncContext*.
 *  Normal use case is:
 *			MpegEncContext * const s = ff_h264_extract_Context(avctx, &h, i);
 *
 *	vOIdx has to be in the range 0..MAX_VIEW_COUNT otherwise vOIdx = 0 is taken.
 *
 *  return MpegEncContext
 */
MpegEncContext* ff_h264_extract_Context(const AVCodecContext * avctx, H264Context** h, int voidx);

/** INIT H264Context
 * 	Does the same initialization then extract_H264Context()
 * 	without extracting H264Context out of AVCodecContext.
 */
void init_H264Context(H264Context *h);

/** Adds the SPS to the buffer of each MVC context.
 *
 * 	If a SPS with same id exit, the old SPS is deleted and replaced.
 * 	Depending on NAL unit type (NAL_SUB_SPS, NAL_SPS, NAL_SPS_EXT)
 * 	the parameter set is put to sub_sps-buffer or sps_buffer.
 *
 * 	If the SPS is no sub SPS, it is activated.
 *
 *	@param h	pointer to the H264Context. WARNING: this context have to be a main and not a threaded context!
 *	@param sps	pointer to the SPS, which should be stored in buffer.
 *	@return  \li{Override: 1} \li{Success: 0} \li{Error: -1}
 */
int save_SPS(H264Context *h, SPS* sps);

/** ACTIVATE SPS
 * 	Activates the SPS.
 * 	Depending on the current NAL unit type, the SPS is taken from the sps_buffer or the sub_sps_buffer.
 *
* 	@param h		pointer to the H264Context. !WARNING: this context have to be a main and not a threaded context!
 *	@param sps_id	id of the SPS, which should be activated.
 *	@return SPS*    \li{Success: the pointer to the activated SPS} \li{Error: 0}
 */
SPS* activate_SPS(H264Context *h, uint sps_id);

/** Adds the PPS to the buffer of each MVC context.
 *
 * 	If a PPS with same id exit, the old PPS is deleted and replaced.
 *
 *	@param h		pointer to the H264Context. !WARNING: this context have to be a main and not a threaded context!
 *	@param pps		pointer to the PPS, which should be stored in buffer.
 *	@param pps_id	id of the PPS. (necessary, since PPS does not store their own id)
 *	@return  		\li{Override: 1} \li{Success: 0} \li{Error: -1}
 */
int save_PPS(H264Context *h, PPS* pps, uint pps_id);

/** ACTIVATE PPS
 * 	Activates the PPS from pps_buffer and the SPS with id = pps.sps_id.
 * 	For SPS activation @see activate_SPS() is used.
 *
 * 	@param h		pointer to the H264Context. !WARNING: this context have to be a main and not a threaded context!
 *	@param pps_id	id of the PPS. (necessary, since PPS does not store their own id)
 *	@return PPS*    \li{Success: the pointer to the activated PPS} \li{Error: 0}
 */
PPS* activate_PPS(H264Context *h, uint pps_id);

// ==================================================================== //
//  							CLAUSES									//
// ==================================================================== //

/** 7.3.2.1.2
 * 	Sequence parameter set extension RBSP syntax
 *
 * 	seq_parameter_set_extension_rbsp()
 */
void ff_h264_decode_sps_ext(H264Context *h, SPS *sps);


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
