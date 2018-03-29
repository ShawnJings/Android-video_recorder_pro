#include "./gpu_texture_cache.h"

#define LOG_TAG "GPUTextureCache"

/******************* GPUTextureCache class *******************/
GPUTextureCache::GPUTextureCache() {
}

GPUTextureCache::~GPUTextureCache() {
}

//初始化静态成员
GPUTextureCache* GPUTextureCache::instance = new GPUTextureCache();
GPUTextureCache* GPUTextureCache::GetInstance() {
	return instance;
}

void GPUTextureCache::destroy() {
	map<string, list<GPUTexture*> >::iterator queueItor;
	for (queueItor = textureQueueCache.begin(); queueItor != textureQueueCache.end(); ++queueItor) {
		list<GPUTexture*>::iterator texItor;
		for (texItor = (queueItor->second).begin(); texItor != (queueItor->second).end(); ++texItor) {
			GPUTexture* texture = *texItor;
			texture->dealloc();
			delete texture;
		}
		(queueItor->second).clear();
	}
	textureQueueCache.clear();
}

GPUTexture* GPUTextureCache::fetchTexture(int width, int height) {
	GPUTexture* texture = NULL;
	string queueKey = getQueueKey(width, height);
	map<string, list<GPUTexture*> >::iterator itor;
	itor = textureQueueCache.find(queueKey);
	if (itor != textureQueueCache.end()) {
		//找到了
		if ((itor->second).size() > 0) {
			texture = (itor->second).front();
			(itor->second).pop_front();
		} else {
			texture = new GPUTexture();
			texture->init(width, height);
		}
	} else {
		//没找到
		LOGI("没找到 textureQueue");
		list<GPUTexture*> textureQueue;
		texture = new GPUTexture();
		texture->init(width, height);
		textureQueueCache[queueKey] = textureQueue;
	}
//	LOGI("texture from cache : %d", texture->getTexId());
	return texture;
}

void GPUTextureCache::returnTextureToCache(GPUTexture* texture) {
//	LOGI("texture return cache : %d", texture->getTexId());
	string queueKey = getQueueKey(texture->getWidth(), texture->getHeight());
	map<string, list<GPUTexture*> >::iterator itor;
	itor = textureQueueCache.find(queueKey);
	if (itor != textureQueueCache.end()) {
		(itor->second).push_back(texture);
	} else {
		LOGI("unreachable code...");
	}
}

string GPUTextureCache::getQueueKey(int width, int height) {
	string queueKey = "tex_";
	char widthBuffer[8];
	sprintf(widthBuffer, "%d", width);
	queueKey.append(string(widthBuffer));
	queueKey.append("_");
	char heightBuffer[8];
	sprintf(heightBuffer, "%d", height);
	queueKey.append(string(heightBuffer));
	return queueKey;
}

/******************* GPUTexture class *******************/
GPUTexture::GPUTexture() {
}

GPUTexture::~GPUTexture() {
}

void GPUTexture::init(int width, int height) {
	this->width = width;
	this->height = height;
	this->createTexture(width, height);
	referenceCount = 0;
}

void GPUTexture::dealloc(){
	glDeleteTextures(1, &texId);
}

GLuint GPUTexture::createTexture(GLsizei width, GLsizei height) {
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

void GPUTexture::lock() {
	referenceCount++;
}

void GPUTexture::unLock() {
	referenceCount--;
	if (referenceCount < 1) {
		GPUTextureCache::GetInstance()->returnTextureToCache(this);
	}
}

void GPUTexture::clearAllLocks() {
	referenceCount = 0;
}
