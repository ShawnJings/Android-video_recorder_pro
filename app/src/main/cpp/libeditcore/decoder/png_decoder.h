#ifndef PNG_DECODER_H
#define PNG_DECODER_H

extern "C" {
	#include "./image.h"
}
#include "../platform_dependent/platform_common.h"
#include "./rgba_frame.h"

/**
 * PNG图像解码
 */
class PngDecoder {
private:
	FILE* pngFile;

	byte* compressedData;
	int actualSize;
public:
	PngDecoder();
	/** 开启解码线程 **/
	virtual ~PngDecoder();
	/** 打开本地文件不需要传递 探针的参数以及重试策略 **/
	int openFile(char *pngFilePath);
	RGBAFrame* getRGBAFrame();
	void closeFile();

};

#endif //PNG_DECODER_H

