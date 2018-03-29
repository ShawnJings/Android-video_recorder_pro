static char* SMOOTH_SKIN_HORIZONTAL_VERTEX_SHADER =
"attribute vec4 position;    			   												\n"
"attribute vec2 texcoord;   	   															\n"
"uniform float texelWidthOffset;	   														\n"
"uniform float texelHeightOffset;	   													\n"
"const int GAUSSIAN_SAMPLES = 13;	   														\n"
"varying vec2 textureCoordinate;															\n"
"varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];											\n"
"																						\n"
"void main(void)               															\n"
"{                            															\n"
"   gl_Position = position;  															\n"
"   textureCoordinate = texcoord.xy;														\n"
"   							  															\n"
"   // Calculate the positions for the blur												\n"
"   int multiplier = 0;																	\n"
"   vec2 blurStep;																		\n"
"   vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);				\n"
"   for (int i = 0; i < GAUSSIAN_SAMPLES; i++)											\n"
"   {																					\n"
"   		multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));									\n"
"   		blurStep = float(multiplier) * singleStepOffset;									\n"
"   		blurCoordinates[i] = texcoord.xy + blurStep;										\n"
"   }																					\n"
"}                            															\n";

//Shader.frag文件内容
static char* SMOOTH_SKIN_HORIZONTAL_FRAG_SHADER =
"precision lowp float;																		\n"
"uniform sampler2D yuvTexSampler;															\n"
"const lowp int GAUSSIAN_SAMPLES = 13;														\n"
"varying vec2 textureCoordinate;																\n"
"varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];												\n"
"																							\n"
"void main()																					\n"
"{																							\n"
"	lowp float sum = 0.0;																\n"
"   lowp vec4 fragColor=texture2D(yuvTexSampler,textureCoordinate);							\n"
"																							\n"


"    sum += texture2D(yuvTexSampler, blurCoordinates[0]).g * 0.0042;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[1]).g * 0.0134;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[2]).g * 0.0344;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[3]).g * 0.0715;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[4]).g * 0.1204;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[5]).g * 0.1647;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[6]).g * 0.1828;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[7]).g * 0.1647;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[8]).g * 0.1204;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[9]).g * 0.0715;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[10]).g * 0.0344;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[11]).g * 0.0134;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[12]).g * 0.0042;              \n"



"	gl_FragColor = vec4(fragColor.rgb, sum);									\n"
"}																							\n";


static char* SMOOTH_SKIN_VERTICAL_VERTEX_SHADER =
"attribute vec4 position;    			   												\n"
"attribute vec2 texcoord;   	   															\n"
"uniform float texelWidthOffset;	   														\n"
"uniform float texelHeightOffset;	   													\n"
"const int GAUSSIAN_SAMPLES = 13;	   														\n"
"varying vec2 textureCoordinate;															\n"
"varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];											\n"
"																						\n"
"void main(void)               															\n"
"{                            															\n"
"   gl_Position = position;  															\n"
"   textureCoordinate = texcoord.xy;														\n"
"   							  															\n"
"   // Calculate the positions for the blur												\n"
"   int multiplier = 0;																	\n"
"   vec2 blurStep;																		\n"
"   vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);				\n"
"   for (int i = 0; i < GAUSSIAN_SAMPLES; i++)											\n"
"   {																					\n"
"   		multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));									\n"
"   		blurStep = float(multiplier) * singleStepOffset;									\n"
"   		blurCoordinates[i] = texcoord.xy + blurStep;										\n"
"   }																					\n"
"}                            															\n";


static char* SMOOTH_SKIN_VERTICAL_FRAG_SHADER =
"precision lowp float;																	\n"
"uniform sampler2D yuvTexSampler;														\n"
"const lowp int GAUSSIAN_SAMPLES = 13;													\n"
"varying vec2 textureCoordinate;															\n"
"varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];											\n"
"uniform mediump float mask_curve_progress;                                                     \n"
"uniform mediump float soft_light_progress;                                                  \n"
"uniform mediump float s_curve_progress;                                               \n"
"uniform mediump float satuRatio;                                               \n"
"																						\n"
"void main()																				\n"
"{																						\n"
"	float sum = 0.0;															\n"
"   lowp vec4 fragColor=texture2D(yuvTexSampler,textureCoordinate);						\n"
"																						\n"


"    sum += texture2D(yuvTexSampler, blurCoordinates[0]).a * 0.0042;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[1]).a * 0.0134;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[2]).a * 0.0344;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[3]).a * 0.0715;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[4]).a * 0.1204;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[5]).a * 0.1647;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[6]).a * 0.1828;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[7]).a * 0.1647;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[8]).a * 0.1204;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[9]).a * 0.0715;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[10]).a * 0.0344;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[11]).a * 0.0134;              \n"
"    sum += texture2D(yuvTexSampler, blurCoordinates[12]).a * 0.0042;              \n"

"																						\n"
"   sum = fragColor.g - sum + 0.5; \n"
//sum 的结果是highpass filter

//三次强光混合
"   if (sum <=0.5) \n"
"   { \n"
"       sum = 128.0 * sum * sum * sum * sum * sum * sum * sum * sum ; \n"
"   }\n"
"   else \n"
"   { \n"
"       sum = 1.0 - sum; \n"
"       sum = 1.0 - 128.0 * sum * sum * sum * sum * sum * sum * sum * sum ; \n"
"   } \n"
//蒙版曲线
"   lowp vec4 fc1 = fragColor; \n"
"   fc1.rgb = fragColor.rgb * (mask_curve_progress*fragColor.rgb+(1.0*(1.0 - fragColor.rgb))) + fragColor.rgb * (1.0 - mask_curve_progress);  \n"
"   gl_FragColor = vec4(mix(fc1, fragColor, sum).rgb,1.0);  \n"

//提亮
"mediump vec4 overlay = vec4(0.6, 0.6, 0.6, soft_light_progress);     \n"
"gl_FragColor.rgb = gl_FragColor.rgb * (overlay.a * gl_FragColor.rgb + (2.0 * overlay.rgb * (1.0 - gl_FragColor.rgb ))) + gl_FragColor.rgb * (1.0 - overlay.a);\n"
//饱和度
"   float gray = gl_FragColor.r * 0.3 + gl_FragColor.g * 0.59 + gl_FragColor.b * 0.11; \n"
"   gl_FragColor.rgb = gl_FragColor.rgb * satuRatio + (1.0 - satuRatio) * gray; \n"
//S对比度
"   gl_FragColor.rgb = s_curve_progress * gl_FragColor.rgb*(gl_FragColor.rgb+(2.0*gl_FragColor.rgb*(1.0-gl_FragColor.rgb))) + gl_FragColor.rgb*(1.0-s_curve_progress);  \n"



"}																						\n";
