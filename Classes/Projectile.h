#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "cocos2d.h"
#include "GameCharacter.h"
#include "ProjectileMovingPart.h"
#include "ProjectileShapePart.h"

using namespace cocos2d;

/**
*	 “子弹类型”，主要是希望普通的远程武器与不同的“子弹”搭配来构成
*   角色的不同普通远程攻击方式
*/
enum ProjectileTypeEnum
{
    PROJECTILE_TYPE_GALAXO_BALL,                // 闪电球
    PROJECTILE_TYPE_ZEUS_FLY_HAMMER,            // 宙斯的飞锤
    PROJECTILE_TYPE_PIG_FLY_KNIFE,              // 野猪飞刀
    PROJECTILE_TYPE_NIU_FLY_SHIELD,             // 牛的飞刀
    PROJECTILE_TYPE_THEIF_FLY_DAGGER,           // 刺客飞匕首
};

/**
*	“子弹状态”的标志位，当全为0的时候就直接删除
*/
enum ProjectileStateEnum
{
    PROJECTILE_STATE_UPDATEMOVEMENT =   0x00001,        // 子弹正在移动的更新
    PROJECTILE_STATE_UPDATE         =   0x00002,        // 除了位移意外的更新
};

/**
*	 远程攻击武器发出的“子弹”
*/
class Projectile : public Ref
{
public:
    /**
    *	只能通过这个接口创建子弹，这里作为类似工厂的作用，这里提供一个传入
    *   自定义数据的能力，在create中应该有能力根据不同的类型使用这个数据
    */
    static Projectile* create(GameCharacterAttribute& att, ProjectileTypeEnum type, void* extraData, int ownerId);

    /**
    *	在此处完成位移更新，不再使用原来的继承方式，而是采用
    */
    void updateMovement(float dm);

    /**
    *	返回显示节点，这个将被加入地图的显示列表 
    */
    CC_SYNTHESIZE(ProjectileShapePart*, m_shapePart, ShapePart);

    /**
    *	在此处完成除了位移更新以外的其他更新
    */
    virtual void update(float dm) = 0;

protected:
    /**
    *	子弹必须得位置和速度还有就是发射者的属性一起跟随，在撞击后用来计算属性变化的
    *   子弹发射后就与发射者无关了
    */
    Projectile(GameCharacterAttribute& att);

    virtual ~Projectile();

    /**
    *	子弹推进器，用来管理子弹的移动部分
    */
    CC_SYNTHESIZE(ProjectileMovingPart*, m_movingPart, MovingPart);

    /**
    *	关于子弹的状态 
    */
    CC_SYNTHESIZE(int, m_state, State);

    /**
    *	该子弹的拥有者，就是发射者
    */
    CC_SYNTHESIZE_READONLY(int, m_ownerId, OwnerId);

    bool canUpdate() { return (m_state & PROJECTILE_STATE_UPDATE) != 0; }
    bool canUpdateMovement() { return (m_state & PROJECTILE_STATE_UPDATEMOVEMENT) != 0; }
    bool canRemove() { return m_state == 0; }
    void updateOff() { m_state &= ~PROJECTILE_STATE_UPDATE; }
    void updateMovementOff() { m_state &= ~PROJECTILE_STATE_UPDATEMOVEMENT; }

    /**
    *	关于用于计算的发射者的各种属性 
    */
    GameCharacterAttribute      m_senderAtt;

private:
    ProjectileTypeEnum          m_projectileType;               // “子弹”类型
    static int m_count;
    CC_SYNTHESIZE_READONLY(int, m_projectileId, ProjectileId);  // 唯一标识

    // 在移动部分调用updateMovement之前的位置
    Vec2                        m_lastPos;
};

#endif