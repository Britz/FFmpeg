/*
 * h264_mvc_config.h
 * Copyright (c) 2012 Jochen Britz
 */

/**
 * @file
 * H.264 / AVC / MPEG4 part10 codec /
 * SVC/MVC extension specific functions
 * @author: Jochen Britz
 */

#ifndef H264MVCCONFIG_H_
#define H264MVCCONFIG_H_

// ==================================================================== //
//  				      DEFINED VARIABLES			    				//
// ==================================================================== //

#define MAX_VIEW_COUNT		8	///< maximum amount of views, could be up to 1024
#define LINK_DPB			1	///< 0:separate DPBs per view, 1:one common DPBs for all views

#define INTER_PIC_REF		8	///< reference mask for inter-view references

#endif /* H264MVCCONIG_H_ */
