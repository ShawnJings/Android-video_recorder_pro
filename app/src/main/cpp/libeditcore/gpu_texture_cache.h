#ifndef VIDEO_GPU_TEXTURE_CACHE_H
#define VIDEO_GPU_TEXTURE_CACHE_H

#include <map>
#include <list>
#include <string>
using namespace std;

#include "./platform_dependent/platform_common.h"
#include "./platform_dependent/platform_gl.h"

class GPUTexture {
private:
	int width;
	int height;
	GLuint texId;
	int referenceCount;

	GLuint createTexture(GLsizei width, GLsizei height);

public:
	GPUTexture();
	~GPUTexture();

	int getWidth(){
		return width;
	};
	int getHeight(){
		return height;
	};
	GLuint getTexId(){
		return texId;
	};
	void init(int width, int height);
	void dealloc();
	void lock();
	void unLock();
	void clearAllLocks();
};

class GPUTextureCache {
private:
	GPUTextureCache(); //注意:构造方法私有
	static GPUTextureCache* instance; //惟一实例

	map<string, list<GPUTexture*> > textureQueueCache;

	string getQueueKey(int width, int height);

public:
	static GPUTextureCache* GetInstance(); //工厂方法(用来获得实例)
	virtual ~GPUTextureCache();

	GPUTexture* fetchTexture(int width, int height);
	void returnTextureToCache(GPUTexture* texture);

	void destroy();
};

#endif // VIDEO_GPU_TEXTURE_CACHE_H
