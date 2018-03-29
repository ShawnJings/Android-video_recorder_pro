#include "./sox_compressor_effect.h"

#define LOG_TAG "SoxCompressorEffect"

SoxCompressorEffect::SoxCompressorEffect(double sampleRate, int channels) :
		SoxBaseEffect(sampleRate, channels) {
	compressorChain = NULL;
}

SoxCompressorEffect::~SoxCompressorEffect() {
}

static int input_drain(sox_effect_t * effp, sox_sample_t * obuf, size_t * osamp) {
//	LOGI("input_drain success...");
	//note that:永远正常结束，但是当外边程序主动调用AudioEffectProcessor的destroy的时候停止
	*osamp = SoxCompressorEffect::size;
	int i = 0;
	for (; i < *osamp; i++) {
		obuf[i] = ((sox_sample_t) (SoxCompressorEffect::preProcessSamples[i]) << 16);
	}
	return SOX_SUCCESS;
}

static int output_flow(sox_effect_t *effp LSX_UNUSED, sox_sample_t const * ibuf, sox_sample_t * obuf LSX_UNUSED, size_t * isamp, size_t * osamp) {
	if (*isamp) {
		int i = 0;
		for (; i < *isamp; i++) {
			SoxCompressorEffect::responseData[i] = (short) (ibuf[i] >> 16);
		}
	}
	*osamp = 0;
//	LOGI("output_flow success...");
	return *isamp == 0 ? SOX_SUCCESS : SOX_EOF;
}

static sox_effect_handler_t const * input_handler(void) {
	static sox_effect_handler_t handler = { "input", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY, NULL, NULL, NULL, input_drain, NULL, NULL, 0 };
	return &handler;
}

static sox_effect_handler_t const * output_handler(void) {
	static sox_effect_handler_t handler = { "output", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY | SOX_EFF_PREC, NULL, NULL, output_flow, NULL, NULL, NULL, 0 };
	return &handler;
}

short* SoxCompressorEffect::preProcessSamples = NULL;
int SoxCompressorEffect::size = 0;
short* SoxCompressorEffect::responseData = NULL;

/** 初始化全局变量 **/
int SoxCompressorEffect::init(int bufferSize){
	responseData = new short[bufferSize * 2];
	return SoxBaseEffect::init(bufferSize);
}

/** 销毁 **/
void SoxCompressorEffect::destroy(){
	if(NULL != responseData){
		delete[] responseData;
		responseData = NULL;
	}
}

int SoxCompressorEffect::initChain(SOXFilterChainParam* filterChainParam) {
	SoxBaseEffect::initSignal();
	SOXCompressorFilterParam* compressorFilterParam = CompressorParamBuilder::GetInstance()->buildCompressorFilterParamWithFactor(0.02f, 0.04f, -20, 1.1, -25, 1.3, 0);
	this->initCompressorChain(compressorFilterParam);
	return 1;
}

int SoxCompressorEffect::process(short* audioSamples, int sizeParam) {
	size = sizeParam;
	preProcessSamples = audioSamples;
	sox_flow_effects(compressorChain, NULL, NULL);
	memcpy(audioSamples, responseData, size * 2);
	return size;
}

void SoxCompressorEffect::destroyChain() {
	if (NULL != compressorChain) {
		sox_delete_effects_chain(compressorChain);
	}
}

void SoxCompressorEffect::initCompressorChain(SOXCompressorFilterParam* compressorFilterParam) {
	compressorChain = sox_create_effects_chain(&encoding, &encoding);

	sox_effect_t * e = sox_create_effect(input_handler());
	assert(sox_add_effect(compressorChain, e, &signal, &signal) == SOX_SUCCESS);
	free(e);

	if (NULL != compressorFilterParam && compressorFilterParam->isValid()) {
		int compaundArgs = 5;
		char* attackRelease = new char[20];
		compressorFilterParam->getAttackRelease(&attackRelease);
		char* functionTransforTable = new char[100];
		compressorFilterParam->getFunctionTransforTable(&functionTransforTable);
		char* gain = new char[10];
		compressorFilterParam->getGain(&gain);
		char* initialVolume = new char[10];
		compressorFilterParam->getInitialVolume(&initialVolume);
		char* delay = new char[10];
		compressorFilterParam->getDelay(&delay);
		char * compaundArgv[] = { attackRelease, functionTransforTable, gain, initialVolume, delay };
		LOGI("compaundArgv is %s %s %s %s %s \n", compaundArgv[0], compaundArgv[1], compaundArgv[2], compaundArgv[3], compaundArgv[4]);
		e = sox_create_effect(sox_find_effect("compand"));
		assert(sox_effect_options(e, compaundArgs, compaundArgv) == SOX_SUCCESS);
		assert(sox_add_effect(compressorChain, e, &signal, &signal) == SOX_SUCCESS);
		for (int i = 0; i < compaundArgs; i++) {
			delete[] compaundArgv[i];
		}
		free(e);
	}

	e = sox_create_effect(output_handler());
	assert(sox_add_effect(compressorChain, e, &signal, &signal) == SOX_SUCCESS);
	free(e);
}
