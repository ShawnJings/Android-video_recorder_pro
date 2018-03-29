//磨皮效果器的 vertex shader 文件内容  
static const char* WHITENING_EFFECT_HOR_VERTEX_SHADER = 
	"attribute highp vec4 position;	\n"
	"attribute highp vec2 texcoord;	\n"
	"varying highp vec2 fragTexCoord;	\n"
	"void main()		\n"
	"{	\n"
     	"fragTexCoord = texcoord;	\n"
      	"gl_Position = position;	\n"
	"}\n";

