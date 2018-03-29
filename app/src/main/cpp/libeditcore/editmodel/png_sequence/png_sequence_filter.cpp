#include "./png_sequence_filter.h"

#define LOG_TAG "PngSequenceFilter"

/* PngSequenceFilter class */
PngSequenceFilter::PngSequenceFilter() {
	texCache = NULL;
	texId = -1;
	decoder = NULL;
}

PngSequenceFilter::PngSequenceFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	texCache = NULL;
	texId = -1;
	decoder = NULL;
}

PngSequenceFilter::~PngSequenceFilter() {
	clear();
}

bool PngSequenceFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	decoder = new PngDecoder();
	curPosition = sequenceIn/1000000.0f;
	currentIndex = 0;
	LOGI("PngSequenceFilter::onInit");
	ParamVal val;
	bool suc = this->getFilterParamValue(string(PNG_SEQUENCE_PARAM_ID_DIR_PATH), val);
	if (suc) {
		char *dirPath = (char *)val.strVal.c_str();
		LOGI("get success, dirPath:%s", dirPath);
		if(this->parseScene(dirPath)){
			ParamVal paramValGifWidth;
			paramValGifWidth.type = EffectParamTypeInt;
			paramValGifWidth.u.intVal = width;
			this->setFilterParamValue(PNG_SEQUENCE_PARAM_ID_PNG_WIDTH, paramValGifWidth);

			ParamVal paramValGifHeight;
			paramValGifHeight.type = EffectParamTypeInt;
			paramValGifHeight.u.intVal = height;
			this->setFilterParamValue(PNG_SEQUENCE_PARAM_ID_PNG_HEIGHT, paramValGifHeight);
		} else{
			suc = false;
		}
	}
	return suc;
}

void PngSequenceFilter::onRenderPre(float pos) {
	if(NULL == texCache){
		//Create Texture
		texCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
		if (texCache) {
			texCache->lock();
			texId = texCache->getTexId();
		}
		LOGI("texId is %d", texId);
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = texId;
		this->setFilterParamValue(PNG_SEQUENCE_PARAM_ID_TEXTURE_ID, paramValue);
	}
	if (pos >= curPosition) {
		char indexBuffer[8];
		sprintf(indexBuffer, "%d.png", currentIndex%imageCount);
		string pngName = baseName;
		pngName.append(imageName).append(string(indexBuffer));
//		LOGI("png name is %s width is %d height %d", pngName.c_str(), width, height);
		if (decoder->openFile((char*) pngName.c_str()) > 0) {
			RGBAFrame* frame = decoder->getRGBAFrame();
//			LOGI("frame width is %d", frame->width);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->pixels);
			delete frame;
			decoder->closeFile();
		}
		curPosition += intervalInSec;
		currentIndex++;
	} else if(curPosition - pos >= 2 * intervalInSec){
		curPosition = sequenceIn/1000000.0f;
		this->onRenderPre(pos);
	}
}

void PngSequenceFilter::clear() {
	//删除纹理以及decoder 释放资源
	if (texCache)
		texCache->unLock();
	if(decoder){
		delete decoder;
		decoder = NULL;
	}
}

string PngSequenceFilter::readInputTestFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file)
		return std::string("");
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::string text;
	char* buffer = new char[size + 1];
	buffer[size] = 0;
	if (fread(buffer, 1, size, file) == (unsigned long) size)
		text = buffer;
	fclose(file);
	delete[] buffer;
	return text;
}

bool PngSequenceFilter::parseScene(char* sceneDirPath) {
	this->baseName = string(sceneDirPath);
	std::string path(sceneDirPath);
	path.append(string("/config.json"));
	LOGI("theme path is %s", path.c_str());
	/* just for json parser test */
	string content = readInputTestFile(path.c_str());
	if (content.empty()) {
		LOGE("json file read empty, do something");
	}
	LOGI("json content is %s", content.c_str());

	ChangbaJson::Reader reader;
	ChangbaJson::Value value;
	bool suc = reader.parse(content, value);
	if (suc) {
		width = value["width"].asInt();
		LOGI("width is %d", width);
		height = value["height"].asInt();
		LOGI("height is %d", height);
		imageName = value["imageName"].asString();
		LOGI("imageName is %s", imageName.c_str());
		imageCount = value["imageCount"].asInt();
		LOGI("imageCount is %d", imageCount);
		intervalInSec = value["imageIntervalInSec"].asFloat();
		LOGI("intervalInSec is %f", intervalInSec);
	} else {
		LOGE("parse json failed");
	}
	return suc;
}
