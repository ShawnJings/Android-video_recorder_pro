#ifndef VIDEO_EFFECT_PARAMS_H
#define VIDEO_EFFECT_PARAMS_H

/* 该文件描述了所有视频滤镜、转场参数名称以及该参数取值范围 */
/* 参数类型枚举：
 //enum EffectParamType
 //{
 //	EffectParamTypeInvalid 		= -1,
 //	EffectParamTypeArbitrary 	= 0,
 //	EffectParamTypeInt 			= 1,
 //	EffectParamTypeFloat 		= 2,
 //	EffectParamTypeBoolean 		= 3,
 //	EffectParamTypeString 		= 4,
 //	EffectParamTypeColor 		= 5,
 //	EffectParamTypePosition2D 	= 6,
 //	EffectParamTypePosition3D 	= 7,
 //  EffectParamTypePath			= 8,
 //	EffectParamTypeCount
 //};
 * 没有类型注释的暂定为不可被客户端或者设计访问的参数，比如纹理坐标、物体坐标
 */

/****    Image effect begin     ****/
/* contrast */																			// 滤镜名字
#define IMAGE_EFFECT_ZOOM_VIEW_RATIO									"image effect zoom view ratio"
#define IMAGE_BASE_EFFECT_NAME									    "image base effect name"
#define PLAYER_CONTRAST_FILTER_NAME									"Video Player Contrast"
#define CONTRAST_FILTER_NAME											"Contrast"
#define CONTRAST_EFFECT_SLOPE_CHANGED								"Contrast Slope Changed"
#define CONTRAST_EFFECT_SLOPE										"Contrast Slope"
#define CONTRAST_PARAM_ID_VERTEX_COORDS								"vertex coordinate"				// 物体坐标
#define CONTRAST_PARAM_ID_TEXTURE_COORDS								"texture coordinate"			// 纹理坐标
#define CONTRAST_PARAM_ID_TEST										"test"							// 测试值，整型，最小值0，最大值100，缺省值100
#define GRAY_SCALE_FILTER_NAME										"GrayScale"
#define VIGNETTE_FILTER_NAME											"Vignette"
#define TONE_CURVE_FILTER_NAME										"ToneCurve"
#define TONE_CURVE_FILTER_ACV_BUFFER_CHANGED							"ToneCurve acv buffer changed"
#define TONE_CURVE_FILTER_ACV_BUFFER									"ToneCurve acv buffer"
#define TONE_CURVE_FILTER_ACV_BUFFER_SIZE							"ToneCurve acv buffer size"
#define THIN_WHITENING_FILTER_NAME									"ThinWhitening"
#define THIN_WHITENING_FILTER_TYPE_FLAG								"ThinWhitening type"
#define WHITENING_FILTER_NAME										"Whitening"
#define BEAUTIFY_FACE_FILTER_NAME                                   "Beautify Face"
#define BEAUTIFY_FACE_COOL_FILTER_NAME                              "Beautify Face Cool"
#define THIN_BEAUTIFY_FACE_FILTER_NAME                              "Thin Beautify Face"
#define THIN_EFFECT_VFLIP_FLAG                                      "Thin Vflip flagTHIN_EFFECT_VFLIP_FLAG"
#define WHITENING_COOL_FILTER_NAME                                  "Whitening Cool"
#define WHITENING_FILTER_TEXTURE_WIDTH								"Whitening texture width"
#define WHITENING_FILTER_TEXTURE_HEIGHT								"Whitening texture height"
#define WHITENING_FILTER_AMPLITUDE									"Whitening amplitude"
#define WHITENING_FILTER_EDGER										"Whitening edger"
#define WHITENING_BILATERAL_RADIUS_FACTOR                           "Whitening Bilateral Radius Factor"
#define WHITENING_SOFT_LIGHT_BLEND_R                                "Whitening Soft Blend R"
#define WHITENING_SOFT_LIGHT_BLEND_G                                "Whitening Soft Blend G"
#define WHITENING_SOFT_LIGHT_BLEND_B                                "Whitening Soft Blend B"
#define WHITENING_SOFT_LIGHT_BLEND_A                                "Whitening Soft Blend A"
#define HUE_EFFECT_HUE_ANGLE_CHANGED                                "hue_effect_hue_angle_changed"
#define HUE_EFFECT_HUE_ANGLE                                        "hue_effect_hue_angle"
#define SMOOTH_SKIN_EFFECT_PARAM_CHANGED                            "smooth_skin_effect_param_changed"
#define MASK_CURVE_PROGRESS                                         "mask_curve_progress"
#define SOFT_LIGHT_PROGRESS                                         "soft_light_progress"
#define S_CURVE_PROGRESS                                            "s_curve_progress"
#define SATU_RATIO                                                  "satuRatio"
#define SHARPEN_EFFECT_SHARPNESS                                    "sharpen_effect_sharpness"
#define SHARPEN_EFFECT_WIDTH                                        "sharpen_effect_width"
#define SHARPEN_EFFECT_HEIGHT                                       "sharpen_effect_height"
#define MIX_FILTER_NAME												"Mix Effect"
#define MIX_FILTER_TEXTURE_ID										"MixTextureId"
#define MIX_FILTER_MIX_PROGRESS										"Mix progress"
#define IMAGE_EFFECT_GROUP_TEXTURE_WIDTH								"Effect Group Texture width"
#define IMAGE_EFFECT_GROUP_TEXTURE_HEIGHT							"Effect Group Texture height"

/****    Image effect ending     ****/

/* gif_scene */																			// 滤镜名字
#define SCREEN_SCENE_FILTER_NAME						"screen_scene"
#define GIF_SCENE_FILTER_NAME							"gif_scene"
#define GIF_SCENE_PARAM_ID_HFLIP						"hflip"							// hflip，bool值，缺省值false
#define GIF_SCENE_PARAM_ID_X							"x"								// 中心点的x坐标，float值，最小值-2，最大值2，缺省值0，居中
#define GIF_SCENE_PARAM_ID_Y							"y"								// 中心点的y坐标，float值，最小值-2，最大值2，缺省值0，居中
#define GIF_SCENE_PARAM_ID_WIDTH						"width"							// 宽度占百分比，float值，最小值0，最大值1，缺省值0.5
#define GIF_SCENE_PARAM_ID_ANGLE						"angle"							// 旋转角度，float值，最小值0，最大值360，缺省45
#define GIF_SCENE_PARAM_ID_PATH							"gif path"						// gif路径，必填
#define GIF_SCENE_PARAM_ID_TEXTURE_ID					"texture id"					// 纹理ID，客户端和主题不需要管
#define GIF_SCENE_PARAM_ID_GIF_WIDTH					"gif width"						// gif的实际宽度，客户端和主题不需要管，整型
#define GIF_SCENE_PARAM_ID_GIF_HEIGHT					"gif height"					// gif的实际高度，客户端和主题不需要管，整型

/* 动图改为png序列来实现 */
#define PNG_SEQUENCE_FILTER_NAME							"png_sequence"
#define PNG_SEQUENCE_PARAM_ID_DIR_PATH					"scene dir path"					// 道具的文件夹，必填
#define PNG_SEQUENCE_PARAM_ID_HFLIP						"hflip"							// hflip，bool值，缺省值false
#define PNG_SEQUENCE_PARAM_ID_X							"x"								// 中心点的x坐标，float值，最小值-2，最大值2，缺省值0，居中
#define PNG_SEQUENCE_PARAM_ID_Y							"y"								// 中心点的y坐标，float值，最小值-2，最大值2，缺省值0，居中
#define PNG_SEQUENCE_PARAM_ID_WIDTH						"width"							// 宽度占百分比，float值，最小值0，最大值1，缺省值0.5
#define PNG_SEQUENCE_PARAM_ID_ANGLE						"angle"							// 旋转角度，float值，最小值0，最大值360，缺省45
#define PNG_SEQUENCE_PARAM_ID_TEXTURE_ID					"texture id"						// 纹理ID，客户端和主题不需要管
#define PNG_SEQUENCE_PARAM_ID_PNG_WIDTH					"png width"						// png的实际宽度，客户端和主题不需要管，整型
#define PNG_SEQUENCE_PARAM_ID_PNG_HEIGHT					"png height"						// png的实际高度，客户端和主题不需要管，整型

/* mask_alpha */																		// 滤镜名字
#define MASK_AHPHA_FILTER_NAME							"mask_alpha"
#define MASK_ALPHA_PARAM_ID_PATH						"mask alpha path"				// mask alpha路径
#define MASK_ALPHA_PARAM_ID_TEXTURE_ID					"mask alpha texture id"					// 纹理ID

/** overlay_scene **/
#define VIDEO_OVERLAY_SCENE_FILTER_NAME						"video_overlay_scene"
#define VIDEO_BLACK_BOARD_PATH							"vodeo black board path"
#define OVERLAY_SCENE_FILTER_NAME						"overlay_scene"
#define BLACK_BOARD_PIC_PATH							"black board pic path"
#define BLACK_BOARD_TEXTURE_ID							"black board tex id"
#define OVERLAY_MAP_PIC_PATH							"overlay map pic path"
#define OVERLAY_MAP_TEXTURE_ID							"overlay map tex id"
#define AMARO_MAP_PIC_PATH								"amaro map pic path"
#define AMARO_MAP_TEXTURE_ID							"amaro map tex id"

/** header_scene **/
#define HEADER_FADE_SCENE_FILTER_NAME						"header_fade_scene"
#define HEADER_FADE_VIDEO_PATH								"header fade video path"
#define	HEADER_FADE_VIDEO_FADE_OUT_TIME_IN_SECS				"header fade screen fade out in secs"
#define HEADER_FADE_VIDEO_TEXTURE_ID							"header fade video tex id"
#define HEADER_FADE_VIDEO_FADE_OUT_PROGRESS					"header fade fade out progress"

/** header_scene **/
#define HEADER_SCREEN_SCENE_FILTER_NAME						"header_screen_scene"
#define HEADER_VIDEO_PATH									"header video path"
#define HEADER_VIDEO_TEXTURE_ID								"header video tex id"
#define	HEADER_SCREEN_PIC_PATH								"header screen pic path"
#define HEADER_SCREEN_TEXTURE_ID								"header screen tex id"
#define HEADER_SCENE_PARAM_ID_PIXELS							"header scene pixels"

/** text_scene **/
#define TEXT_SCENE_FILTER_NAME							"text_scene"
#define TEXT_SCENE_PARAM_ID_PIXELS						"text scene pixels"
#define TEXT_SCENE_PARAM_ID_WIDTH						"text scene width"	//整个画布的宽度
#define TEXT_SCENE_PARAM_ID_HEIGHT						"text scene height"	//整个画布的高度
#define TEXT_SCENE_PARAM_ID_TEXTURE_ID					"text scene tex id"
#define TEXT_SCENE_PARAM_ID_ALPHA_PROGRESS				"text scene alpha progress"
#define TEXT_SCENE_PARAM_ID_FADE_IN_END_TIME				"text scene fade in end time" //整个文字fade in的结束时间
#define TEXT_SCENE_PARAM_ID_FADE_OUT_START_TIME			"text scene fade out start time" //整个文字fade out的开始时间
#define TEXT_SCENE_PARAM_ID_TEXT_LEFT					"text scene text left"	//客户端向画布绘制文字的left
#define TEXT_SCENE_PARAM_ID_TEXT_TOP						"text scene text top"	//客户端向画布绘制文字的top
#define TEXT_SCENE_PARAM_ID_TEXT_WIDTH					"text scene text width"	//客户端向画布绘制文字宽度
#define TEXT_SCENE_PARAM_ID_TEXT_HEIGHT					"text scene text height"//客户端向画布绘制文字高度
#define TEXT_SCENE_PARAM_ID_TEXT_ALIGNMENT				"text scene text alignment"//客户端向画布绘制文字的对齐方式 -1 0 1 分别是左对齐，居中对齐，右对齐
#define TEXT_SCENE_PARAM_ID_TEXT_COLOR					"text scene text color"//绘制文字的颜色
#define TEXT_SCENE_PARAM_ID_TEXT_SHADOW_RADIUS			"text scene text shadow radius"//绘制文字的阴影半径
#define TEXT_SCENE_PARAM_ID_TEXT_SHADOW_XOFFSET			"text scene text shadow x offset"//绘制文字的x轴偏移
#define TEXT_SCENE_PARAM_ID_TEXT_SHADOW_YOFFSET			"text scene text shadow y offset"//绘制文字的y轴偏移
#define TEXT_SCENE_PARAM_ID_TEXT_SHADOW_COLOR			"text scene text shadow color"//绘制文字的阴影颜色
#define TEXT_SCENE_PARAM_ID_TEXT_SIZE					"text scene text size"//绘制文字的大小
#define TEXT_SCENE_PARAM_ID_TEXT_TYPE					"text scene text type"//文字类型 0代表songName 1代表nickName

/** transition_scene **/
#define TRANSITION_SCENE_FILTER_NAME					"transition_scene"
#define TRANSITION_SCENE_PARAM_ID_PATH					"transition video path"
#define TRANSITION_SCENE_PARAM_ID_DURATION				"transition duration"
#define TRANSITION_SCENE_PARAM_ID_TYPE					"transition type"
#define TRANSITION_SCENE_PARAM_ID_TEXTURE_ID			"transition scene tex id"
#define TRANSITION_SCENE_PARAM_ID_PROGRESS			    "transition scene progress"

/** blur_scene **/
#define BLUR_SCENE_FILTER_NAME								"blur_scene"
#define SELECTIVE_BLUR_SCENE_FILTER_NAME						"selective_blur_scene"
#define BLUR_SCENE_PARAM_ID_BLUR_ASCEND_FLAG		    			"blur scene ascend flag"
#define BLUR_SCENE_PARAM_ID_PROGRESS				    			"blur scene blurSize progress"

/** saturation_scene **/
#define SATURATION_SCENE_FILTER_NAME							"saturation_scene"
#define SATURATION_SCENE_PARAM_ID_SATURATION			    		"saturation scene saturation"

/** trailer_scene **/
#define TRAILER_SCENE_FILTER_NAME							"trailer_scene"
#define TRAILER_SCENE_PARAM_ID_PATH							"trailer scene overlay path"
#define TRAILER_SCENE_PARAM_ID_WIDTH							"trailer scene width"
#define TRAILER_SCENE_PARAM_ID_HEIGHT						"trailer scene height"
#define TRAILER_SCENE_PARAM_ID_TEXT_LEFT						"trailer scene text left"
#define TRAILER_SCENE_PARAM_ID_TEXT_TOP						"trailer scene text top"
#define TRAILER_SCENE_PARAM_ID_TEXT_WIDTH					"trailer scene text width"	//客户端向画布绘制文字宽度
#define TRAILER_SCENE_PARAM_ID_TEXT_HEIGHT					"trailer scene text height"//客户端向画布绘制文字高度
#define TRAILER_SCENE_PARAM_ID_TEXT_ALIGNMENT				"trailer scene text alignment"//客户端向画布绘制文字的对齐方式 -1 0 1 分别是左对齐，居中对齐，右对齐
#define TRAILER_SCENE_PARAM_ID_TEXT_COLOR					"trailer scene text color"//绘制文字的颜色
#define TRAILER_SCENE_PARAM_ID_TEXT_SIZE						"trailer scene text size"//绘制文字的大小
#define TRAILER_SCENE_PARAM_ID_TEXT_TYPE						"trailer scene text type"//文字类型 0代表songName 1代表nickName
#define TRAILER_SCENE_PARAM_ID_DURATION						"trailer duration"
#define TRAILER_SCENE_PARAM_ID_TEXTURE_ID					"trailer scene tex id"
#define TRAILER_SCENE_PARAM_ID_PROGRESS				    		"trailer scene progress"
#define TRAILER_SCENE_PARAM_ID_PIXELS						"trailer scene pixels"

#define IMAGE_4_MIRROR_FILTER_NAME											"Image 4 Mirror Effect"
#define IMAGE_2_MIRROR_FILTER_NAME											"Image 2 Mirror Effect"
#define IMAGE_COLOR_INVERT_FILTER_NAME										"Color Invert Effect"
#define IMAGE_TILT_SHIFT_FILTER_NAME                                        "Tilt Shift Effect"
#define IMAGE_BLUR_MIRROR_FILTER_NAME                                       "Blur Mirror Effect"
#define IMAGE_SOUL_STUFF_FILTER_NAME                                        "Soul Stuff Effect"
#define IMAGE_CAROUSEL_FILTER_NAME                                          "Carousel Effect"
#define TIME_STRETCH_FILTER_NAME                                            "Time Stretch Effect"
#define IMAGE_TEXTURE_WIDTH                                                 "Image Texture Width"
#define IMAGE_TEXTURE_HEIGHT                                                "Image Texture Height"
#define IMAGE_TRANSFORM_SCALE_WIDTH                                         "Image Transform Scale Width"
#define IMAGE_TRANSFORM_ROTATE_ANGLE                                        "Image Transform Rotate Angle"
#define IMAGE_TRANSFORM_TRANSLATE_XOFFSET                                   "Image Transform Translate XOffset"
#define IMAGE_TRANSFORM_TRANSLATE_YOFFSET                                   "Image Transform Translate YOffset"


#endif // VIDEO_EFFECT_PARAMS_H
