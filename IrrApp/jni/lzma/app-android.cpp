#include <stdio.h>
#include <cstdlib>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <android-receiver.h>
#include <android-logger.h>
#include <importgl.h>
#include <pthread.h>

#include <irrlicht.h>
#include <os.h>

using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};
#define MIN(x, y) ((x) < (y)) ? (x) : (y);
#define MAX(x, y) ((x) > (y)) ? (x) : (y);
#define NUM_G 3;
#define NUM_TV 4;
#define NUM_HM 2;
#define NUM_MA 5;
#define NUM_S 2;

extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern stringc gSdCardPath;
bool UseHighLevelShaders = false;
bool UseCgShaders = false;

ISceneManager* smgr = NULL;
IGUIEnvironment* guienv = NULL;
IAnimatedMesh* mesh = NULL;
ISceneNode* node = NULL;
IAnimatedMeshSceneNode *nodeSydney = NULL;
ICameraSceneNode* camera = NULL;
IGUIStaticText* diagnostics = NULL;
SAppContext context;
AndroidEventReceiver *receiver;
scene::ISceneNodeAnimator* animPa;


video::ITexture** images  = NULL;
gui::IGUIFont* font = NULL;

u32 then;
u32 nFrame;
s32 lastFPS;
f32 startFlag;
const f32 MOVEMENT_SPEED = 5.f;
core::vector3df cameraPosition;
core::vector3df cameraRotation;
int *frameNo;
char filename[50];
char invfilename[50];
f32 frameElapsed;
s32 activeNode;
s32 frameCounter = 0;
s32 currentNode = 0;
s32 playframe = 0;
u16 num_me[5] = {3,5,7,5,2};
u16 titleIndex[5] = {2,3,2,4,1};
bool playflag,flag,flag1;
s32 menuNum = 4;
s32 glassOffset = 3;

//静态模型的路径
stringc meshPath[5][7] = {
		{"/Irrlicht/3D_3D.3ds","/Irrlicht/3D_pad.3ds","/Irrlicht/3D_title.3ds","/Irrlicht/3D_padm.3ds"},
		{"/Irrlicht/mytv_whitepad.3ds","/Irrlicht/mytv_MY.3ds","/Irrlicht/why.3ds","/Irrlicht/hua_padm.3ds","/Irrlicht/mytv_title.3ds"},
		{"/Irrlicht/hua_pad.3ds","/Irrlicht/hua_padm.3ds","/Irrlicht/homemedia_title.3ds","/Irrlicht/hua_padm.3ds","/Irrlicht/hua_padm.3ds","/Irrlicht/hua_padm.3ds","/Irrlicht/hua_padm.3ds"},
		{"/Irrlicht/myapp_box.3ds","/Irrlicht/myapp_player.3ds","/Irrlicht/myapp_controler.3ds","/Irrlicht/myapp_My.3ds","/Irrlicht/myapp_App.3ds"},
		{"/Irrlicht/setting_lun.3ds","/Irrlicht/setting_title.3ds"}
};
/*
stringc banPath[5] = {
	"/Irrlicht/3D.png",
	"/Irrlicht/mytv.png",
	"/Irrlicht/homemedia.png",
	"/Irrlicht/myapp.png",
	"/Irrlicht/settings.png"
};

stringc invPath[5] = {
	"/Irrlicht/3Dinv.png",
	"/Irrlicht/mytvinv.png",
	"/Irrlicht/homemediainv.png",
	"/Irrlicht/myappinv.png",
	"/Irrlicht/settingsinv.png"
};
*/

//模型贴图的路径
stringc banPath[5] = {
	"/Irrlicht/glass1.jpg",
	"/Irrlicht/glass1.jpg",
	"/Irrlicht/glass1.jpg",
	"/Irrlicht/glass1.jpg",
	"/Irrlicht/glass1.jpg"
};

//模型贴图倒影的路径
stringc invPath[5] = {
	"/Irrlicht/invglass1.jpg",
	"/Irrlicht/invglass1.jpg",
	"/Irrlicht/invglass1.jpg",
	"/Irrlicht/invglass1.jpg",
	"/Irrlicht/invglass1.jpg"
};

//动态模型的路径
stringc aniPath[5] = {
			"/Irrlicht/mytv.b3d",
//			"Irrlicht/game.3DS",
			"/Irrlicht/homemedia.b3d",
			"/Irrlicht/try.b3d",
			"/Irrlicht/settings.b3d",
			"/Irrlicht/ninja.b3d"
};

//active node玻璃块贴图及其倒影的路径
stringc glassPath[2] = {
		"/Irrlicht/glass1.jpg",
		"/Irrlicht/invglass1.jpg",
};

//设定入口位置、大小等属性的父类
class SceneLayoutBase {
protected:
	s32 amount;                             //入口个数
	irr::core::vector3df *position;         //入口位置
	irr::core::vector3df *scale;            //入口大小
	irr::core::vector3df *rotation;         //入口旋转角度
	irr::core::vector3df cameraPosition;    //相机（视点）位置
	irr::core::vector3df cameraRotation;    //相机（视点）旋转角度
public:
	SceneLayoutBase(s32 amount) {
		this->amount = amount;
		this->position = new irr::core::vector3df[amount];
		this->scale = new irr::core::vector3df[amount];
		this->rotation = new irr::core::vector3df[amount];
	}
	virtual ~SceneLayoutBase() {
		delete [] position;
		delete [] scale;
		delete [] rotation;
	};

	//返回入口个数
	s32 getAmount() {
		return amount;
	}

	//返回入口位置
	irr::core::vector3df *fixedPosition() {
		return position;
	}

	//返回入口大小
	irr::core::vector3df *fixedScale() {
		return scale;
	}

	//返回入口旋转角度
	irr::core::vector3df *fixedRotation() {
		return rotation;
	}

	//返回相机（视点）位置
	irr::core::vector3df getCameraPosition() {
		return cameraPosition;
	}

	//返回相机（视点）旋转角度
	irr::core::vector3df getCameraRotation() {
		return cameraRotation;
	}
};


//设定入口位置、大小等属性的子类
class OutCyclicLayout : public SceneLayoutBase {
public:
	OutCyclicLayout(s32 amount,f32 w=80,f32 h=80,f32 t=2)
		: SceneLayoutBase(amount) {
			this->cylinderR = 100;
			this->width = w;
			this->height = h;
			this->disY = height;
			this->offsetY = height * .5f;
			this->thickness = t;
			this->deltaOmega = irr::core::PI * 2 / amount ;
			for (s32 i = 0; i < amount; i++) {
				float omega = 0;
				//this->offsetY = 0;
				if (i){
					omega = (amount - i) * deltaOmega;      //计算第i个入口的相对偏移角度
				}
				if(i == 1 or i==3){
					position[i] = irr::core::vector3df(sin(omega) * cylinderR*1.5, 0, cos(omega) * cylinderR);
				}
				else {
					position[i] = irr::core::vector3df(sin(omega) * cylinderR, 0, cos(omega) * cylinderR);
				}
				scale[i] = irr::core::vector3df(width, height, thickness);
				rotation[i] = irr::core::vector3df(0, omega * 180 / irr::core::PI, 0);
			}
			rotation[0] = irr::core::vector3df(0, 0, 0);
			cameraPosition = irr::core::vector3df(0, 0, 180);	//位置向量，非别为（左右，上下，后前）
	}
	virtual ~OutCyclicLayout() {};
private:
	f32 disY;       //Y轴上的位置偏移量
	f32 offsetY;    //Y轴上的位置偏移量
	f32 cylinderR;  //入口排布的圆的半径
	f32 width;      //入口的宽度
	f32 height;     //入口的高度
	f32 thickness;  //入口的厚度
	f32 deltaOmega; //入口的相对偏移角度
	f32 offsetOmega;
};

//记录入口node信息的类
class WallNode {
public:
	WallNode(scene::ISceneManager* smgr, SceneLayoutBase *layout, s32 pos = 0, bool active = false, f32 animationLast = 1000.f) {
		//sceme::IMeshManipulator *manipulator = smgr->getMeshManipulator();
		this->smgr = smgr;
		this->layout = layout;
		this->pos = pos;
		this->active = active;
		this->posTimeLast = this->animationLast = animationLast;
		this->posTimeToGo = 0;
		this->scaTimeElapse = this->scaTimeLast = 500.f;
		this->parentNode = smgr->addCubeSceneNode(1, 0 , ID_IsNotPickable);
		//this->parentNode->setVisible(false);
		this->sceneNode = smgr->addCubeSceneNode(1, parentNode, IDFlag_IsPickable | IDFlag_IsHighlightable | (pos << 2));
		this->invNode = smgr->addCubeSceneNode(1,parentNode,ID_IsNotPickable);
		this->selector = smgr->createTriangleSelector(this->sceneNode->getMesh(), this->sceneNode);
		this->sceneNode->setTriangleSelector(this->selector);
		//this->selector->drop();
		//std::cout << this->sceneNode->getMaterialCount();
	}
	~WallNode() {};

	//计算wallNodes在一帧中的位置、大小和旋转角度
	void touch(f32 time) {
		if (posTimeToGo) {              //处于运动中
			if (posTimeToGo <= time) {  //运动应当停止
				posTimeToGo = 0;        //剩余运动时间置0
				this->posTimeLast =  this->animationLast;
				this->parentNode->setPosition(posDes);  //固定好节点位置
			} else {                    //运动结束
				posTimeToGo -= time;    //剩余运动时间置减去已流逝时间time
				if (posTimeToGo > posTimeLast / 2) {    //前期运动效果
					f32 t2 =  (posTimeLast - posTimeToGo) * (posTimeLast - posTimeToGo);
					this->parentNode->setPosition(posSrc + posVel * (posTimeLast - posTimeToGo) + posAcc * (t2 / 2));
					//std::cout << this->sceneNode->getPosition().X << ", " <<  this->sceneNode->getPosition().Y << ", " <<  this->sceneNode->getPosition().Z << "\n";
				} else {                //后期运动效果
					f32 t2 =  posTimeToGo * posTimeToGo;
					this->parentNode->setPosition(posDes - posVel * posTimeToGo - posAcc * (t2 / 2));
				}
			}
		}
		if (this->scaTimeElapse < scaTimeLast) {    //运动还未停止
			this->scaTimeElapse += time;            //已运动时间加上已流逝时间time
			if (this->scaTimeElapse > this->scaTimeLast) {  //运动应当停止
				this->scaTimeElapse = this->scaTimeLast;
				this->sceneNode->setScale(this->scaDes);    //设置入口的目标大小
				this->parentNode->setRotation(this->rotDes);//设置入口的目标旋转角度
			} else {        //运动继续
				this->sceneNode->setScale(this->scaSrc + this->scaVel * this->scaTimeElapse);       //计算入口此刻大小
				this->parentNode->setRotation(this->rotSrc + this->rotVel * this->scaTimeElapse);   //计算parentNode此刻旋转角度
			}
		}
	}

	//返回玻璃块节点
	scene::ISceneNode *getSceneNode() {
		return this->sceneNode;
	}

	//返回父节点
	scene::ISceneNode *getParentNode() {
		return this->parentNode;
	}

	//返回玻璃块倒影节点
	scene::ISceneNode *getInvNode(){
		return this->invNode;
	}

	//返回标题节点
	scene::ISceneNode *getTitleNode(){
		return this->titleNode;
	}

	//返回标题倒影节点
	scene::ISceneNode *getInvtitleNode(){
		return this->invtitleNode;
	}

	//返回动画模型节点
	scene::IAnimatedMeshSceneNode *getAniNode(){
		return this->aniNode;
	}


	scene::ISceneNode *getBanNode() {
		return this->banNode;
	}

	//初始化帧数目
	void initAF() {
		aniframe = 0;
	}

	//初始化动画节点
	void initAniNode(u16 i_ani){
		this->aniNode = smgr->addAnimatedMeshSceneNode(
				smgr->getMesh((gSdCardPath + aniPath[i_ani]).c_str()),
				this->getParentNode(),
				ID_IsNotPickable);  //参数为路径、父节点及ID,ID设置为不可选中
		this->aniNode->setAnimationSpeed(0);            //初始动画速度为0，为静止状态
		this->aniNode->getMaterial(0).Lighting = true;  //使得光照有效，从而可以看见模型
		this->aniNode->setVisible(false);               //模型设置为不可见
		this->aniNode->setLoopMode(false);              //模型动画不做循环
		//this->aniNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	}

	//初始化标题节点
	void initTtileNode(u16 ind){
	    //标题节点以parentNode为父节点，不可选中
		this->titleNode = smgr->addCubeSceneNode(1,this->parentNode,ID_IsNotPickable);
		this->invtitleNode = smgr->addCubeSceneNode(1,this->parentNode,ID_IsNotPickable);
		//this->titleNode->setVisible(false);
		//this->invtitleNode->setVisible(false);
	}

	//停止模型的动画
	void removePlay() {
		this->aniNode->removeAnimators();
	}

	//玻璃块运动初始化
	//参数src：源parentNode的index
	//参数src：parentNode将移到的位置对应的index
	void initAnimation(s32 src, s32 des) {
		if (this->posTimeToGo) {                                //表示玻璃块处于运动中
			this->posSrc = this->parentNode->getPosition();     //获取当前parentNode点位置为下一帧运动的初始位置
			this->posTimeToGo = this->posTimeLast = MAX(300.f, posTimeToGo);
		}
		else {                                                  //表示运动还未开始
			this->posSrc = layout->fixedPosition()[src];        //src对应的节点位置为初始位置
			this->posTimeToGo = 1000.f;                         //运动时间设置为1000ms
		}
		this->posDes = layout->fixedPosition()[des];            //des对应的节点位置为目标位置
		this->posAcc = (this->posDes - this->posSrc) * (4/ this->animationLast / this->animationLast);  //设置运动的速度
		this->posVel = (this->posDes - this->posSrc) * ((1 / this->posTimeLast) - (this->posTimeLast / this->animationLast / this->animationLast));

		this->scaTimeElapse = 0;
		this->scaSrc = this->sceneNode->getScale();             //获取初始节点大小
		this->scaDes = layout->fixedScale()[des];               //获取目标节点大小
		this->scaVel = (this->scaDes - this->scaSrc) / this->scaTimeLast;   //计算大小变化单位量
		this->rotSrc = this->parentNode->getRotation();         //获取初始节点旋转角度
		this->rotDes = layout->fixedRotation()[des];            //获取目标节点旋转角度
		irr::core::vector3df rotDis = (this->rotDes - this->rotSrc);        //计算旋转角度差向量
		//std::cout << rotDis.X << " " << rotDis.Y << " " << rotDis.Z << "\n";

		//将旋转角度控制在-180到180度的区间内
		while (rotDis.X > 180) rotDis.X -= 360;
		while (rotDis.Y > 180) rotDis.Y -= 360;
		while (rotDis.Z > 180) rotDis.Z -= 360;
		while (rotDis.X < -180) rotDis.X += 360;
		while (rotDis.Y < -180) rotDis.Y += 360;
		while (rotDis.Z < -180) rotDis.Z += 360;

		this->rotVel = rotDis / this->scaTimeLast;              //计算旋转角度变化单位量

	}

	//玻璃块运动初始化
	//参数src：源parentNode的位置
	//参数src：parentNode将移到的位置
	//此函数重载void initAnimation(s32 src, s32 des)，具体实现与之基本一致
	void initAnimation(core::vector3df src, core::vector3df des) {
			if (this->posTimeToGo) {
				this->posSrc = this->parentNode->getPosition();
				this->posTimeToGo = this->posTimeLast = MAX(300.f, posTimeToGo);
			} else {
				this->posSrc = src;
				this->posTimeToGo = 1000.f;
			}
			this->posDes = des;
			this->posAcc = (this->posDes - this->posSrc) * (4/ this->animationLast / this->animationLast);
			this->posVel = (this->posDes - this->posSrc) * ((1 / this->posTimeLast) - (this->posTimeLast / this->animationLast / this->animationLast));

			this->scaTimeElapse = 0;
			this->scaSrc = this->sceneNode->getScale();
			this->scaDes = this->scaSrc;
			this->scaVel = (this->scaDes - this->scaSrc) / this->scaTimeLast;
			this->rotSrc = this->parentNode->getRotation();
			this->rotDes = this->rotSrc;
			irr::core::vector3df rotDis = (this->rotDes - this->rotSrc);
			//std::cout << rotDis.X << " " << rotDis.Y << " " << rotDis.Z << "\n";
			while (rotDis.X > 180) rotDis.X -= 360;
			while (rotDis.Y > 180) rotDis.Y -= 360;
			while (rotDis.Z > 180) rotDis.Z -= 360;
			while (rotDis.X < -180) rotDis.X += 360;
			while (rotDis.Y < -180) rotDis.Y += 360;
			while (rotDis.Z < -180) rotDis.Z += 360;
			this->rotVel = rotDis / this->scaTimeLast;

	}

	//设置动画模型及标题节点可视性
	void setVis(bool isVis) {
		aniNode->setVisible(isVis);
		titleNode->setVisible(isVis);
		invtitleNode->setVisible(isVis);
	}

	//转向next方向时更新节点index及active node
	void labelNext(s32 amount, s32 times) {
		this->pos += times;             //向后移动times单位时index加上times
		while (this->pos >= amount) {
			this->pos -= amount;        //循环计算防止溢出
		}
		if (!this->pos) active = true;  //更新active node
		else active = false;
	}

	////转向previous方向时更新节点index及active node
	void labelPrev(s32 amount, s32 times) {
		this->pos -= times;             //向前移动times单位时index加上times
		while (this->pos < 0) {
			this->pos += amount;        //循环计算防止溢出
		}
		if (!this->pos) active = true;  //更新active node
		else active = false;
	}

    //动画帧数自加一个单位
	void incAF(){
		this->aniframe++;
	}

	//获取当前动画对应帧号
	s32 getAF(){
		return this->aniframe;
	}

	//获取当前节点位置
	s32 getPos() {
		return this->pos;
	}

	//返回当前节点是否为active node
	bool isActive() {
		return this->active;
	}

	//获取剩余运动时间
	f32 getPosTimeToGo(){
		return posTimeToGo;
	}
private:
	s32 pos;            //入口位置
	bool active;        //入口是否为active node
	f32 animationLast;
	f32 posTimeLast;    //运动时长
	f32 posTimeToGo;    //剩余运动时间
	f32 scaTimeLast;    //运动时长
	f32 scaTimeElapse;  //已运动时间
	//s32 *pnframe;
	s32 aniframe;                   //当前动画的帧数
	irr::core::vector3df posSrc;    //初始位置
	irr::core::vector3df posDes;    //目标位置
	irr::core::vector3df posAcc;    //运动速度大小
	irr::core::vector3df posVel;
	irr::core::vector3df scaSrc;    //初始大小
	irr::core::vector3df scaDes;    //目标大小
	irr::core::vector3df scaVel;
	irr::core::vector3df rotSrc;    //初始旋转角度
	irr::core::vector3df rotDes;    //目标旋转角度
	irr::core::vector3df rotVel;
	scene::ISceneManager *smgr;
	SceneLayoutBase *layout;            //记录位置大小信息
	scene::ISceneNode *parentNode;      //其他节点的父节点，为其他节点提供位置坐标参照
	scene::IMeshSceneNode *invNode;     //入口玻璃块倒影节点
	scene::IMeshSceneNode *sceneNode;   //入口玻璃块节点
	scene::IMeshSceneNode *banNode;     //动画模型节点
	scene::IMeshSceneNode *titleNode;   //标题节点
	scene::IMeshSceneNode *invtitleNode;//标题倒影节点
	//scene::IAnimatedMeshSceneNode **playNode;
	scene::IAnimatedMeshSceneNode *aniNode;//动画模型节点
	scene::ITriangleSelector *selector; //触控选择器
};

//使wallNodes向next方向转动times单位
void turnNext(SceneLayoutBase *sceneLayout, WallNode **wallNodes, s32 times) {
	for (s32 i = 0; i < sceneLayout->getAmount(); i++) {
		s32 pos = wallNodes[i]->getPos();   //源节点index
		s32 pos1 = pos + times;             //目标节点index
		while (pos1 >= sceneLayout->getAmount()) {
			pos1 -= sceneLayout->getAmount();       //循环防溢出
		}
		wallNodes[i]->initAnimation(pos, pos1);     //初始化节点pos运动到节点pos1所需的信息
		wallNodes[i]->labelNext(sceneLayout->getAmount(), times);   //更新节点index
	}
}

//使wallNodes向previous方向转动times单位
void turnPrev(SceneLayoutBase *sceneLayout, WallNode **wallNodes, s32 times) {
	for (s32 i = 0; i < sceneLayout->getAmount(); i++) {
		s32 pos = wallNodes[i]->getPos();   //源节点index
		s32 pos1 = pos - times;             //目标节点index
		while (pos1 < 0) {
			pos1 += sceneLayout->getAmount();       //循环防溢出
		}
		wallNodes[i]->initAnimation(pos, pos1);     //初始化节点pos运动到节点pos1所需的信息
		wallNodes[i]->labelPrev(sceneLayout->getAmount(), times);   //更新节点index
	}
}

WallNode **wallNodes =NULL;                     //入口节点信息
SceneLayoutBase *sceneLayout = NULL;            //入口位置及大小信息
scene::ISceneCollisionManager *collMan = NULL;  //碰撞管理器
video::SMaterial material;                      //材质
scene::IMeshSceneNode** frontLight;             //active node高亮灯光
scene::IAnimatedMeshSceneNode** mytvNode = NULL;    //mytv特有的node（由于mytv入口采用静态模型，由多个模型节点共同完成动画）
int mytvframe = 0;                              //mytv动画的帧号

//初始化mytv入口的节点
void initMytvNode()
{
	mytvNode = new IAnimatedMeshSceneNode* [4]; //共4个节点
	for(u16 i=0;i<4;++i){
		mytvNode[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh((gSdCardPath + meshPath[1][i]).c_str()),wallNodes[0]->getParentNode(),ID_IsNotPickable);
		mytvNode[i]->setVisible(false);

	}

	//设置每个节点的大小、位置和旋转角度
	mytvNode[0]->setScale(core::vector3df(2,3,3));
	mytvNode[1]->setScale(core::vector3df(2,3,3));
	mytvNode[2]->setScale(core::vector3df(2,3,3));
	mytvNode[0]->setPosition(core::vector3df(0,7,3));
	mytvNode[0]->setRotation(core::vector3df(0,-90,0));
	mytvNode[1]->setPosition(core::vector3df(0,17,8));
	mytvNode[1]->setRotation(core::vector3df(0,-90,0));
	mytvNode[2]->setPosition(core::vector3df(0,-3,6));
	mytvNode[2]->setRotation(core::vector3df(0,-90,0));
	mytvNode[3]->setParent(mytvNode[0]);
	mytvNode[3]->setScale(core::vector3df(1.38,1,1));
	mytvNode[3]->setPosition(core::vector3df(0,-0.5,0.42));
	mytvNode[3]->setRotation(core::vector3df(0,90,0));

}

//mytv动画运动初始化
void startPlayMytv()
{
	for(u16 i=0;i<3;++i) mytvNode[i]->setVisible(true);
	mytvframe = 0;
	nFrame = 0;
}

//mytv动画运动开始
void doPlayMytv()
{
	scene::ISceneNodeAnimator* anim;    //动画变量
	core::vector3df pos_D;              //节点初始位置
	core::vector3df pos_S;              //节点目标位置
	u32 now;
	f32 frameDeltaTime;
	stringc frameFilename;

	if(mytvframe == 0){
		mytvframe++;
		pos_S = mytvNode[1]->getPosition();
		pos_D = core::vector3df(pos_S.X,pos_S.Y+50,pos_S.Z);
		anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 1000);      //动画变量：从pos_S运动到pos_D，耗时1000ms
		if (anim)
		{
			mytvNode[1]->addAnimator(anim);     //节点载入动画
			anim->drop();
		}
		anim = smgr->createRotationAnimator(core::vector3df(0,3,0));    //动画变量：沿（0,3,0）向量旋转
		if (anim)
		{
			mytvNode[0]->addAnimator(anim);     //节点载入动画
			anim->drop();
		}
		anim = smgr->createRotationAnimator(core::vector3df(0,3,0));    //动画变量：沿（0,3,0）向量旋转
		if (anim)
		{
			mytvNode[1]->addAnimator(anim);      //节点载入动画
			anim->drop();
		}
		pos_S = mytvNode[2]->getPosition();
		pos_D = core::vector3df(pos_S.X,pos_S.Y-30,pos_S.Z+60);
		anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 1000);
		if (anim)
		{
			mytvNode[2]->addAnimator(anim);     //节点载入动画
			anim->drop();
		}
		anim = smgr->createRotationAnimator(core::vector3df(0,-3,0));
		if (anim)
		{
			mytvNode[2]->addAnimator(anim);     //节点载入动画
			anim->drop();
		}

	}

	if(mytvNode[0]->getRotation().Y>=90) {  //旋转超过90度，必须停止旋转
		mytvNode[0]->removeAnimators();     //摘除旋转动画
		mytvNode[0]->setRotation(core::vector3df(0,90,0));  //设置旋转角度，即在旋转已超过90度时校正为90度
		mytvNode[3]->setVisible(true);      //节点3可视化，开始播放动画
		now = device->getTimer()->getTime();                //获取当前时间
		frameDeltaTime = (f32)(now - then) / 1000.f;        //获取时间间隔
		then = now;
		frameElapsed += frameDeltaTime;

		if (frameElapsed > 1.f / 24) {      //每秒24帧
			frameElapsed -= 1.f / 24;
			nFrame++;
		    sprintf(filename, "/Irrlicht/frame0/v0%d.jpg",nFrame);
			frameFilename = filename;
			mytvNode[3]->setMaterialTexture(0, driver->getTexture((gSdCardPath+frameFilename).c_str()));    //载入一帧的图
			if(nFrame==95) nFrame = 0;                      //共95帧，循环播放
		}

	}
}

//mytv动画停止
void stopPlayMytv()
{
    //节点位置返回初始位置
	mytvNode[0]->setPosition(core::vector3df(0,7,3));
	mytvNode[0]->setRotation(core::vector3df(0,-90,0));
	mytvNode[1]->setPosition(core::vector3df(0,17,8));
	mytvNode[1]->setRotation(core::vector3df(0,-90,0));
	mytvNode[2]->setPosition(core::vector3df(0,-3,6));
	mytvNode[2]->setRotation(core::vector3df(0,-90,0));

	//去除动画且设置为不可见
	for(u16 i=0;i<4;++i){
		mytvNode[i]->removeAnimators();
		mytvNode[i]->setVisible(false);
	}
	mytvframe = 0;
}

//获取当前时间
static long _getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}


//初始化动画模型节点
//参数ind：需要初始化的节点index
void initPN(u16 ind)
{
	u16 i;
	stringc texfile;
	wallNodes[ind]->initAniNode(ind);
	switch(ind){
		case 0:
			(wallNodes[ind]->getAniNode())->setScale(core::vector3df(0.1,0.1,0.1));
			(wallNodes[ind]->getAniNode())->setRotation(core::vector3df(-90,0,0));
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			(wallNodes[ind]->getAniNode())->setScale(core::vector3df(5,5,5));
			(wallNodes[ind]->getAniNode())->setRotation(core::vector3df(-90,0,0));
			break;
		default: break;
	}

	sprintf(filename, "/Irrlicht/black.jpg");
	texfile = filename;
	(wallNodes[ind]->getParentNode())->setMaterialTexture(0, driver->getTexture((gSdCardPath + texfile).c_str()));  //parentNode材质为纯黑色
	(wallNodes[ind]->getParentNode())->setMaterialFlag(video::EMF_LIGHTING, false);             //设置光照使node可被光照射
	(wallNodes[ind]->getParentNode())->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);       //使用加色透明模式，使黑色的parentNode不可见

	wallNodes[ind]->initTtileNode(ind);                 //初始化标题节点ind

    //设置标题节点的材质、大小、位置和混色模式
	sprintf(filename, "/Irrlicht/title0%d.jpg",ind+1);
	texfile = filename;
	(wallNodes[ind]->getTitleNode())->setMaterialTexture(0, driver->getTexture((gSdCardPath + texfile).c_str()));
	(wallNodes[ind]->getTitleNode())->setScale(core::vector3df(50,25,0));
	(wallNodes[ind]->getTitleNode())->setPosition(core::vector3df(0,-30,0));
	(wallNodes[ind]->getTitleNode())->setMaterialFlag(video::EMF_LIGHTING, false);
	//(wallNodes[ind]->getTitleNode())->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	(wallNodes[ind]->getTitleNode())->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);        //使用加色透明模式，从而图片的黑色背景不可见

	sprintf(filename, "/Irrlicht/invtitle0%d.jpg",ind+1);
	texfile = filename;
	(wallNodes[ind]->getInvtitleNode())->setMaterialTexture(0, driver->getTexture((gSdCardPath + texfile).c_str()));
	(wallNodes[ind]->getInvtitleNode())->setScale(core::vector3df(50,25,0));
	(wallNodes[ind]->getInvtitleNode())->setPosition(core::vector3df(0,-50+glassOffset,0));
	(wallNodes[ind]->getInvtitleNode())->setMaterialFlag(video::EMF_LIGHTING, false);
	(wallNodes[ind]->getInvtitleNode())->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);     //使用加色透明模式，从而图片的黑色背景不可见

}

//初始化入口节点
void initWallNodes()
{
	sceneLayout = new OutCyclicLayout(menuNum);
	wallNodes = new WallNode*[menuNum];
	for (s32 i = 0; i < sceneLayout->getAmount(); i++) {
		if (!i) wallNodes[i] = new WallNode(smgr, sceneLayout, i, true);	//第0个为active
		else wallNodes[i] = new WallNode(smgr, sceneLayout, i);

        //设置入口节点玻璃块的位置、大小、旋转角度、材质和混色模式
		if (wallNodes[i]->getSceneNode()) {
			wallNodes[i]->getParentNode()->setPosition(sceneLayout->fixedPosition()[i]);
			wallNodes[i]->getParentNode()->setRotation(sceneLayout->fixedRotation()[i]);
			//wallNodes[i]->getParentNode()->setScale(irr::core::vector3df(0.1,0.1,0.1));
			//wallNodes[i]->getParentNode()->setVisible(false);

			wallNodes[i]->getSceneNode()->setScale(sceneLayout->fixedScale()[i]);
			wallNodes[i]->getSceneNode()->setMaterialTexture(0, driver->getTexture((gSdCardPath + banPath[i]).c_str()));
			wallNodes[i]->getSceneNode()->setMaterialFlag(video::EMF_LIGHTING, false);
			wallNodes[i]->getSceneNode()->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);    //采用加色透明模式达到透明效果
			//wallNodes[i]->getSceneNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

			initPN(i);
		}
		//设置入口节点玻璃块的倒影
		if(wallNodes[i]->getInvNode()){
			wallNodes[i]->getInvNode()->setPosition(irr::core::vector3df(0,-80+glassOffset,0));
			wallNodes[i]->getInvNode()->setScale(sceneLayout->fixedScale()[i]);
			wallNodes[i]->getInvNode()->setMaterialTexture(0, driver->getTexture((gSdCardPath + invPath[i]).c_str()));
			wallNodes[i]->getInvNode()->setMaterialFlag(video::EMF_LIGHTING, false);
			wallNodes[i]->getInvNode()->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		}
	}
	initMytvNode(); //初始化mytv的nodes
}


void setMaterial(u16 ind)
{
	switch(ind){
		case 0:
			break;
		case 1:
			//wallNodes[ind]->getAniNode()->getMaterial(0).MaterialType = (video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			//wallNodes[ind]->getAniNode()->getMaterial(10).MaterialType = (video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			wallNodes[ind]->getAniNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			break;
		case 2:
			//wallNodes[ind]->getAniNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			wallNodes[ind]->getAniNode()->getMaterial(0).MaterialType = (video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			break;
		case 3:
			wallNodes[ind]->getAniNode()->getMaterial(0).MaterialType = (video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			//wallNodes[ind]->getAniNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			break;
		default: break;
	}
}
//模型动画运动初始化
//参数ind：所初始化节点的index
void startPlay(u16 ind)
{
	u16 i;
	//设置节点的材质、大小
	wallNodes[ind]->getSceneNode()->setMaterialTexture(0, driver->getTexture((gSdCardPath + glassPath[0]).c_str()));
	wallNodes[ind]->getSceneNode()->setScale(irr::core::vector3df(80,80,0));
	wallNodes[ind]->getInvNode()->setMaterialTexture(0, driver->getTexture((gSdCardPath + glassPath[1]).c_str()));
	wallNodes[ind]->getInvNode()->setScale(irr::core::vector3df(80,80,0));
	wallNodes[ind]->setVis(true);   //使节点可见
	wallNodes[ind]->initAF();       //初始化动画帧号

	setMaterial(ind);

	scene::ISceneNodeAnimator* anim;    //平移动画变量
	core::vector3df pos_D,pos_S;        //平移初始位置、目标位置

	if(ind == 0) {
		startPlayMytv();            //初始化mytv node的运动
		//wallNodes[ind]->getAniNode()->setVisible(false);
	}

	/********* animation **********/
	pos_S = wallNodes[ind]->getAniNode()->getPosition();
	pos_D =  core::vector3df(pos_S.X,pos_S.Y,pos_S.Z+0.00001);
	startFlag = pos_D.Z;
	anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 500);   //从pos_S向pos_D平移，耗时500ms
	if (anim)
	{
		(wallNodes[ind]->getAniNode())->addAnimator(anim);      //载入动画
		anim->drop();
	}

	pos_S = wallNodes[ind]->getTitleNode()->getPosition();
	pos_D =  core::vector3df(pos_S.X,pos_S.Y,pos_S.Z+0.00001);
	anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 500);
	if (anim)
	{
		(wallNodes[ind]->getTitleNode())->addAnimator(anim);    //载入动画
		anim->drop();
	}

	pos_S = wallNodes[ind]->getInvtitleNode()->getPosition();
	pos_D =  core::vector3df(pos_S.X,pos_S.Y,pos_S.Z+0.00001);
	anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 500);
	if (anim)
	{
		(wallNodes[ind]->getInvtitleNode())->addAnimator(anim); //载入动画
		anim->drop();
	}

}

//模型运动开始
//参数ind：运动节点的index
void doingPlay(u16 ind)
{
	u16 i;
	u32 now;
	f32 frameDeltaTime;
	stringc frameFilename;
	scene::ISceneNodeAnimator* anim;
	core::vector3df pos_D,pos_S;
	if(ind == 0 && (wallNodes[ind]->getAniNode())->getPosition().Z == startFlag) doPlayMytv();  //ind为零，启动mytv的动画

	if(wallNodes[ind]->getAF() == 0 && (wallNodes[ind]->getAniNode())->getPosition().Z == startFlag){   //startPlay（）初始完毕
		wallNodes[ind]->incAF();    //帧号自加，表示动画已开始
		//if(ind == 0) (wallNodes[ind]->getAniNode())->setAnimationSpeed(300.0f);
		if(ind == 0) {
			wallNodes[ind]->getAniNode()->setVisible(false);        //mytv采用静态模型，固令动态模型不可见
		}
		else {
			pos_S = (wallNodes[ind]->getAniNode())->getPosition();
			pos_D = core::vector3df(pos_S.X,pos_S.Y-10,pos_S.Z+10);
			anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 400);   //从pos_S向pos_D平移，用时400ms
			if (anim)
			{
				(wallNodes[ind]->getAniNode())->addAnimator(anim);      //载入动画
				anim->drop();
			}
			if(ind ==1)(wallNodes[ind]->getAniNode())->setAnimationSpeed(10.0f);    //1号入口，即home media，动画需速度较慢
			else (wallNodes[ind]->getAniNode())->setAnimationSpeed(30.0f);          //设置动画速度
		}

		pos_S = wallNodes[ind]->getTitleNode()->getPosition();
		pos_D =  core::vector3df(0,-18,30);
		anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 400);
		if (anim)
		{
			(wallNodes[ind]->getTitleNode())->addAnimator(anim);    //载入动画
			anim->drop();
		}

		pos_S = wallNodes[ind]->getInvtitleNode()->getPosition();
		pos_D =  core::vector3df(0,-33+glassOffset,30);
		anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 400);
		if (anim)
		{
			(wallNodes[ind]->getInvtitleNode())->addAnimator(anim); //载入动画
			anim->drop();
		}
	}
}

//模型运动停止
//参数ind：停止节点的index
void stopPlay(u16 ind)
{
	/*
	int i;
	for(i=0;i<5;++i){
		if(i!=ind){
			wallNodes[i]->getAniNode()->setVisible(false);
			wallNodes[i]->getSceneNode()->setVisible(false);
		}

		wallNodes[i]->getInvNode()->setVisible(false);
		wallNodes[i]->getTitleNode()->setVisible(false);
		wallNodes[i]->getInvtitleNode()->setVisible(false);
	}
	*/
	if(ind == 0) stopPlayMytv();    //若为mytv则启动其自己的结束动画程序

	//玻璃块材质大小设定
	wallNodes[ind]->getSceneNode()->setMaterialTexture(0, driver->getTexture((gSdCardPath + banPath[ind]).c_str()));
	wallNodes[ind]->getSceneNode()->setScale(irr::core::vector3df(80,80,2));
	wallNodes[ind]->getInvNode()->setMaterialTexture(0, driver->getTexture((gSdCardPath + invPath[ind]).c_str()));
	wallNodes[ind]->getInvNode()->setScale(irr::core::vector3df(80,80,2));
	//wallNodes[ind]->getParentNode()->setVisible(false);
	//wallNodes[ind]->setVis(false);
	wallNodes[ind]->removePlay();   //去除模型动画
	wallNodes[ind]->getAniNode()->setMesh(smgr->getMesh((gSdCardPath + aniPath[ind]).c_str())); //重新载入mesh使得模型停留在第一帧
	wallNodes[ind]->getAniNode()->setPosition(core::vector3df(0,0,0));  //模型节点返回初始位置
	wallNodes[ind]->getAniNode()->setAnimationSpeed(0);                 //模型停止运动
	wallNodes[ind]->getAniNode()->setVisible(false);                    //模型不可见

	scene::ISceneNodeAnimator* anim;
	core::vector3df pos_D,pos_S;

    //为标题节点设置从当前位置返回到初始位置的动画
	pos_S = wallNodes[ind]->getTitleNode()->getPosition();
	pos_D =  core::vector3df(0,-30,0);
	anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 500);   //动画耗时500ms
	if (anim)
	{
		(wallNodes[ind]->getTitleNode())->addAnimator(anim);    //载入动画
		anim->drop();
	}
	pos_S = wallNodes[ind]->getInvtitleNode()->getPosition();
	pos_D =  core::vector3df(0,-50+glassOffset,0);
	anim = smgr->createFlyStraightAnimator(pos_S,pos_D, 500);
	if (anim)
	{
		(wallNodes[ind]->getInvtitleNode())->addAnimator(anim);
		anim->drop();
	}
}

//程序界面初始化
void init2DMovie()
{
	smgr = device->getSceneManager();   //获取scene manager

	initWallNodes();                    //初始化入口节点

	smgr->addCameraSceneNodeFPS();      //加入第一人称视角
	camera = smgr->getActiveCamera();
	//device->getCursorControl()->setVisible(false);

	lastFPS = -1;

	then = device->getTimer()->getTime();   //获取当前时间

	//core::vector3df cameraPosition = camera->getPosition();
	cameraPosition = sceneLayout->getCameraPosition();  //获取视点位置
	cameraRotation = sceneLayout->getCameraRotation();  //获取视点旋转角度
	camera->setPosition(cameraPosition);                //设置视点位置
	camera->setRotation(cameraRotation);                //设置视点旋转角度

	char fname[50];
	stringc movieFilename;

	//创建初始画面的地面材质
	scene::IMeshSceneNode *floor = smgr->addCubeSceneNode(1,0,ID_IsNotPickable);
	floor->setPosition(core::vector3df(0,-32,0));       //地面位置
	floor->setScale(core::vector3df(500,500,0));        //地面大小，可尽量大些
	floor->setRotation(core::vector3df(92,0,0));        //地面旋转角度，约90度使之看上去为水平面
	sprintf(fname, "/Irrlicht/verfloor2.jpg");
	movieFilename = fname;
	floor->setMaterialTexture(0,driver->getTexture((gSdCardPath + movieFilename).c_str())); //载入地面材质
	floor->setMaterialFlag(video::EMF_LIGHTING,false);
	floor->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);   //采用加色透明模式，隐去图片周围的黑色背景
	//floor->setVisible(false);

	/************** title ***************/
	/*
	sprintf(fname, "/Irrlicht/fonthaettenschweiler.bmp");
	movieFilename = fname;
	font = device->getGUIEnvironment()->getFont((gSdCardPath + movieFilename).c_str());
	scene::ISceneNode* title = smgr->addTextSceneNode(font, L"3D User Guide Beta",
	video::SColor(255,255,255,255), 0,core::vector3df(20,12,160));
    */

	//加入环境光，近乎白光使整个场景清晰可见
	smgr->setAmbientLight(video::SColor(0,250,250,250));

    //设置active node的高亮光节点
	frontLight = new scene::IMeshSceneNode*[5];
	for(int i =0;i<menuNum;++i){
		frontLight[i] = smgr->addCubeSceneNode(1, 0,ID_IsNotPickable);  //光节点不可选中
		frontLight[i]->setPosition(core::vector3df(0,0,170));           //设置光节点位置，在视点前方
		frontLight[i]->setScale(core::vector3df(10,10,0));              //设置光节点大小
		//sprintf(fname, "/Irrlicht/light0%d.jpg",i);
		sprintf(fname, "/Irrlicht/light0%d.jpg",i);
		movieFilename = fname;
		frontLight[i]->setMaterialTexture(0,driver->getTexture((gSdCardPath + movieFilename).c_str())); //设置光节点材质
		frontLight[i]->setMaterialFlag(video::EMF_LIGHTING,false);      //使光节点为光照说感知
		//frontLight[i]->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		frontLight[i]->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);//采用加色透明模式，隐去图片周围的黑色背景
		frontLight[i]->setVisible(false);       //暂时关闭光节点
	}

	collMan = smgr->getSceneCollisionManager(); //加入碰撞管理器

    //初始化部分全局变量
	frameNo = new int[sceneLayout->getAmount()];
	for (int k=0; k<sceneLayout->getAmount(); ++k){
		frameNo[k]=1;
	}
	frameElapsed  = 0;
	activeNode = -1;
	frameCounter = 0;
	currentNode = 0;

	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

    //创建触碰接收器
    receiver = new AndroidEventReceiver(context);

    //设备载入接收器
    device->setEventReceiver(receiver);
}
static int counter = 0;
static bool disLight = true;
static int delay = 0;
static bool doDelay = true;

//动画循环播放函数
void nativeDrawIteration2DMovie()
{
	device->run();
	const u32 now = device->getTimer()->getTime();  //获取当前时间

	if (counter == 0) { //未初始化界面，则调用初始化函数
		init2DMovie();
	}

	u16 dir = receiver->getDI();    //获取手势识别到的方向
	core::line3d<f32> ray;          //碰撞管理器用到的从触点向无穷远处延伸的光线

	ray = collMan->getRayFromScreenCoordinates(
            irr::core::position2di(receiver->getX(), receiver->getY()),
            camera);        //ray初始化，为视点和触点所在的直线

	core::vector3df intersection;   //ray和node碰撞的位置
	core::triangle3df hitTriangle;  //碰撞所处的三角面
	scene::ISceneNode * selectedSceneNode =
		collMan->getSceneNodeAndCollisionPointFromRay(
		ray,
		intersection,
		hitTriangle,
		IDFlag_IsPickable, //使得可被选中的节点才能被碰撞识别
		0);                //获取选中的节点

	const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	then = now;
	frameElapsed += frameDeltaTime;
	if (activeNode == -1 || !wallNodes[activeNode]->isActive()) {
		for (s32 i = 0; i < sceneLayout->getAmount(); i++) {
			if(wallNodes[i]->isActive()) {
				activeNode = i;
			}
		}
	}	//get the index of the active node

	if (dir>0) {    //有滑动的手势
		//s32 newNodeId = selectedSceneNode->getID() >> 2;
		s32 newNodeId;
		if(dir==1) newNodeId = (currentNode+1)%menuNum; //向next方向转动
		else newNodeId = (currentNode+3)%menuNum;       //向previous方向转动
		__android_log_print(ANDROID_LOG_INFO, "selectedNode", "ID:%d",newNodeId);
		s32 absOffset = abs(newNodeId + sceneLayout->getAmount() - currentNode) % sceneLayout->getAmount(); //计算转动偏移量
		if (absOffset > (sceneLayout->getAmount() >> 1)) {  //往previous方向旋转
			turnPrev(sceneLayout, wallNodes, absOffset);
		} else {    //往next方向旋转
			turnNext(sceneLayout, wallNodes, sceneLayout->getAmount() - absOffset);
		}

		if(currentNode != newNodeId && !disLight){  //当前节点不为新节点且高亮光未熄灭
			for(int i=0;i<menuNum;++i) {
				frontLight[i]->setVisible(false);   //熄灭高亮光
			}
			disLight = true;                        //高亮光已熄灭
			stopPlay(currentNode);                  //当前节点动画停止
		}
		__android_log_print(ANDROID_LOG_INFO, "selectedNode", "currentNode:%d",currentNode);
		currentNode = newNodeId;                    //当前节点（即新的active node）ID为newNodeId
		receiver->setDI(0);                         //将手势识别方向初始化为0
		doDelay = true;                             //延迟高亮光启动
	}

	else if (receiver->IsKeyPressed() && selectedSceneNode && !disLight) {  //有触屏且选中节点同时高亮光未熄灭
		s32 newNodeId = selectedSceneNode->getID() >> 2;    //获取新active node的ID,即碰撞检测检测到的节点
		s32 absOffset = abs(newNodeId + sceneLayout->getAmount() - currentNode) % sceneLayout->getAmount(); //计算转动偏移量
		if(absOffset == 2) {
			newNodeId = currentNode;
		}											//修复点选后方入口的bug
		else if(absOffset !=0) if (absOffset > (sceneLayout->getAmount() >> 1)) {
			turnPrev(sceneLayout, wallNodes, absOffset);    //往previous方向旋转
		} else {
			turnNext(sceneLayout, wallNodes, sceneLayout->getAmount() - absOffset); //往next方向旋转
		}
		if(currentNode != newNodeId && !disLight){  //当前节点不为新节点且高亮光未熄灭
			for(int i=0;i<menuNum;++i) {
				frontLight[i]->setVisible(false);   //熄灭高亮光
			}
			disLight = true;                        //高亮光已熄灭
			doDelay = true;                         //延迟高亮光启动
			stopPlay(currentNode);                  //当前节点动画停止
		}
		currentNode = newNodeId;                    //当前节点（即新的active node）ID为newNodeId
	}

	for (s32 i = 0; i < sceneLayout->getAmount(); i++) {
		wallNodes[i]->touch(frameDeltaTime * 1000.f);   //设置节点运动后的位置、大小、选择角度等信息（不动时信息不变）
	}
	if(doDelay){
		delay ++;
	}
	/*if(wallNodes[activeNode]->getPosTimeToGo()<= frameDeltaTime * 1000.f && disLight){
		doDelay = true;
	}*/
	if(doDelay&&((int)(wallNodes[activeNode]->getSceneNode()->getRotation().Y)%360 == 0)
                &&(wallNodes[activeNode]->getParentNode()->getPosition().X==0)){    //新active node已转动到最前方
		frontLight[activeNode]->setVisible(true);   //打开高亮光
		startPlay(activeNode);                      //active node的模型动画初始化
		disLight = false;                           //标识高亮光已点亮
		delay = 0;                                  //delay变量归零
		doDelay = false;                            //延迟高亮光关闭
	}
	doingPlay(activeNode);                          //active node的模型动画继续运动
	driver->beginScene(true, true, video::SColor(0,0,0,0)); //场景背景为黑色
	smgr->drawAll();                                //绘制所有内容
	driver->endScene();

	//count变量循环自加
	counter ++ ;
	if (counter > 200000) {
		counter = 1;
	}
	//guienv->drawAll();
}


void nativeDrawIteration()
{
	nativeDrawIteration2DMovie();
}

