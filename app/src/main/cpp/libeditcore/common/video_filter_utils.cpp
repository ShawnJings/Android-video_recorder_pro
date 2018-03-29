#include "./video_filter_utils.h"

BaseVideoEffect * createVideoEffect(const char * filterName) {
	if (0 == strcmp(filterName, SATURATION_SCENE_FILTER_NAME)) {
		SaturationEffect * obj = new SaturationEffect();
		return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, TIME_STRETCH_FILTER_NAME)) {
        TimeStretchEffect * obj = new TimeStretchEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_CAROUSEL_FILTER_NAME)) {
        ImageCarouselEffect * obj = new ImageCarouselEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_SOUL_STUFF_FILTER_NAME)) {
        ImageSoulStuffEffect * obj = new ImageSoulStuffEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_BLUR_MIRROR_FILTER_NAME)) {
        ImageBlurMirrorEffect * obj = new ImageBlurMirrorEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_TILT_SHIFT_FILTER_NAME)) {
        ImageTiltShiftEffect * obj = new ImageTiltShiftEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_COLOR_INVERT_FILTER_NAME)) {
        ImageColorInvertEffect * obj = new ImageColorInvertEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_2_MIRROR_FILTER_NAME)) {
        Image2MirrorEffect * obj = new Image2MirrorEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_4_MIRROR_FILTER_NAME)) {
        Image4MirrorEffect * obj = new Image4MirrorEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, SELECTIVE_BLUR_SCENE_FILTER_NAME)) {
		SelectiveBlurSceneEffect * obj = new SelectiveBlurSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, BLUR_SCENE_FILTER_NAME)) {
		BlurSceneEffect * obj = new BlurSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, MASK_AHPHA_FILTER_NAME)) {
		MaskAlphaEffect * obj = new MaskAlphaEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, TEXT_SCENE_FILTER_NAME)) {
		TextSceneEffect * obj = new TextSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, HEADER_SCREEN_SCENE_FILTER_NAME)) {
		HeaderSceneEffect * obj = new HeaderSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, HEADER_FADE_SCENE_FILTER_NAME)) {
		HeaderFadeSceneEffect * obj = new HeaderFadeSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	}  else if (0 == strcmp(filterName, OVERLAY_SCENE_FILTER_NAME)) {
		OverlaySceneEffect * obj = new OverlaySceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, VIDEO_OVERLAY_SCENE_FILTER_NAME)) {
		VideoOverlaySceneEffect * obj = new VideoOverlaySceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	}  else if (0 == strcmp(filterName, PNG_SEQUENCE_FILTER_NAME)) {
		PngSequenceEffect * obj = new PngSequenceEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, TRANSITION_SCENE_FILTER_NAME)) {
		TransitionSceneEffect * obj = new TransitionSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, TRAILER_SCENE_FILTER_NAME)) {
		TrailerSceneEffect * obj = new TrailerSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, SCREEN_SCENE_FILTER_NAME)) {
		ScreenSceneEffect * obj = new ScreenSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, GIF_SCENE_FILTER_NAME)) {
		GifSceneEffect * obj = new GifSceneEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, CONTRAST_FILTER_NAME)) {
		ContrastEffect * obj = new ContrastEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, PLAYER_CONTRAST_FILTER_NAME)) {
		PlayerContrastEffect * obj = new PlayerContrastEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, GRAY_SCALE_FILTER_NAME)) {
		GrayScaleEffect * obj = new GrayScaleEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, VIGNETTE_FILTER_NAME)) {
		VignetteEffect * obj = new VignetteEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, TONE_CURVE_FILTER_NAME)) {
		ToneCurveEffect * obj = new ToneCurveEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, THIN_WHITENING_FILTER_NAME)) {
		ThinWhiteningEffect * obj = new ThinWhiteningEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, WHITENING_FILTER_NAME)) {
		WhiteningEffect * obj = new WhiteningEffect();
		return static_cast<BaseVideoEffect *>(obj);
    }  else if (0 == strcmp(filterName, WHITENING_COOL_FILTER_NAME)) {
        WhiteningCoolEffect * obj = new WhiteningCoolEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, BEAUTIFY_FACE_FILTER_NAME)) {
        BeautifyFaceEffect * obj = new BeautifyFaceEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, BEAUTIFY_FACE_COOL_FILTER_NAME)) {
        BeautifyFaceCoolEffect * obj = new BeautifyFaceCoolEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, THIN_BEAUTIFY_FACE_FILTER_NAME)) {
        ThinBeautifyFaceEffect * obj = new ThinBeautifyFaceEffect();
        return static_cast<BaseVideoEffect *>(obj);
    } else if (0 == strcmp(filterName, IMAGE_BASE_EFFECT_NAME)) {
		ImageBaseEffect * obj = new ImageBaseEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else if (0 == strcmp(filterName, MIX_FILTER_NAME)) {
		MixEffect * obj = new MixEffect();
		return static_cast<BaseVideoEffect *>(obj);
	} else {
		return new BaseVideoEffect();
	}
}

ModelFilter * createModelFilter(int filterIndex, int64_t sequenceIn, int64_t sequenceOut, char* filterName) {
	if (0 == strcmp(filterName, CONTRAST_FILTER_NAME)) {
		ModelFilter* obj = new ModelFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, SELECTIVE_BLUR_SCENE_FILTER_NAME)) {
		SelectiveBlurSceneFilter* obj = new SelectiveBlurSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, BLUR_SCENE_FILTER_NAME)) {
		BlurSceneFilter* obj = new BlurSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, GIF_SCENE_FILTER_NAME)) {
		ModelDecoderFilter* obj = new ModelDecoderFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, TEXT_SCENE_FILTER_NAME)) {
		TextSceneFilter* obj = new TextSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, HEADER_SCREEN_SCENE_FILTER_NAME)) {
		HeaderSceneFilter* obj = new HeaderSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, HEADER_FADE_SCENE_FILTER_NAME)) {
		HeaderFadeSceneFilter* obj = new HeaderFadeSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, OVERLAY_SCENE_FILTER_NAME)) {
		OverlaySceneFilter* obj = new OverlaySceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, VIDEO_OVERLAY_SCENE_FILTER_NAME)) {
		VideoOverlaySceneFilter* obj = new VideoOverlaySceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, PNG_SEQUENCE_FILTER_NAME)) {
		PngSequenceFilter* obj = new PngSequenceFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, TRANSITION_SCENE_FILTER_NAME)) {
		TransitionSceneFilter* obj = new TransitionSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, TRAILER_SCENE_FILTER_NAME)) {
		TrailerSceneFilter* obj = new TrailerSceneFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, SCREEN_SCENE_FILTER_NAME)) {
		ModelDecoderFilter* obj = new ModelDecoderFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else if (0 == strcmp(filterName, MASK_AHPHA_FILTER_NAME)) {
		ModelMaskAlphaFilter* obj = new ModelMaskAlphaFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	} else {
		ModelFilter* obj = new ModelFilter(filterIndex, sequenceIn, sequenceOut, filterName);
		return static_cast<ModelFilter *>(obj);
	}
}
