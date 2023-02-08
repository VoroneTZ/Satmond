#define mForward  1
#define mBackward 2
#define mTurnRight  4
#define mTurnLeft 8

#define runSpeed  34
#define walkSpeed 8

#define ActionWalk    1
#define ActionRun   2
#define ActionIdle    3
#define ActionFall    4
#define ActionTalk    5
#define ActionJump    6
#define ActionAttack    7


#define WaitingTalk     1
#define Talking       2
#define TalkingOutOfRange 3

#define FallDistance skill28
#define FallVelocity skill29
#define GravityAccel skill30

#define CurrentAction skill31
#define PreviousAction skill32
#define AnimationPercent skill33

#define Running skill34
#define MovementAction skill35

#define Blending skill36
#define BlendingPercent skill37

#define SizeOffset  skill38
#define RadiusSize  skill39

#define TalkingState skill40
#define Isjump    skill41
#define IsAttack    skill42
#define hp skill43
SOUND* snd_fall = "toprak.wav";
SOUND* snd_jump = "toprak.wav";
SOUND* snd_player_hit = "toprak.wav";
SOUND* snd_death_enemy = "toprak.wav";
SOUND* snd_player_hit_enemy = "toprak.wav";
var snd_handle1=0;
var fallsnd;
var isAttack = 0;



ENTITY* Explo;



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Handles the gravity
////////////////////////////////////////////////////////////////////////////////////////////////////////////
function handle_gravity(var typeMovement) {
  VECTOR fromVec;
  VECTOR toVec;

  var downMovement;
  vec_set(fromVec, me.x);
  vec_set(toVec, me.x);
  fromVec.z = fromVec.z - my.SizeOffset;
  toVec.z = toVec.z - 5000;
  if (c_trace(fromVec, toVec, IGNORE_ME | USE_AABB) >= 0) {

    typeMovement &= ~GLIDE;

    my.FallVelocity += (my.GravityAccel) * (time_step);
    downMovement = c_move(me, vector(0, 0, -my.FallVelocity), NULL, typeMovement);
    if (downMovement <= 0) {
      my.FallVelocity  = 0;
      my.FallDistance = 0;
    }
    else
    {
      my.FallDistance += downMovement;
    }

  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Handles entities movement
////////////////////////////////////////////////////////////////////////////////////////////////////////////




function handle_movement(VECTOR * displace, VECTOR * absMovement, var otherFlags) {
  var typeGravity = IGNORE_MODELS | IGNORE_PASSABLE | GLIDE | otherFlags;
  var Ldist_down; var Lspeed_down = 0;

  VECTOR LvFeet;
  vec_for_min(LvFeet, me);
  if (c_trace(my.x, vector(my.x, my.y, my.z - 5000),  IGNORE_ME | IGNORE_PASSABLE | USE_BOX) > 0)
    Ldist_down = my.z + LvFeet.z - target.z;
  else
    Ldist_down = 0;

  if (Ldist_down > -1)
    Ldist_down = clamp(Ldist_down, 0, accelerate(Lspeed_down, 15, 0.1));
  else
    Lspeed_down = 0;


  if (vec_length(displace) != 0) {
    c_move(me, displace, absMovement,  IGNORE_MAPS| IGNORE_MODELS | IGNORE_ME | IGNORE_PASSABLE |GLIDE | otherFlags);
     //  c_move(me, NULL, vector(0, 0, -Ldist_down),   IGNORE_MAPS| IGNORE_MODELS | IGNORE_PASSABLE | GLIDE);
  }
  handle_gravity(typeGravity);

}





////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Handles the animations
////////////////////////////////////////////////////////////////////////////////////////////////////////////
function handle_animation() {
  STRING* animationName;
  var animFactor;
  var blendFactor;
  var animStart = 0;
  var animEnd = 100;
  var animType = ANM_CYCLE;

  switch (me.CurrentAction) {
    case ActionIdle:
      animationName = "stand";
      animFactor = 1;
    //  break;
    case ActionRun:
      animationName = "run";
      animFactor = 10;
    //  break;
    case ActionWalk:
      animationName = "run";
      animFactor = 5;

    //  break;
    case ActionFall:
      animationName = "falling";
      animFactor = 1;
      animType = 0;
      //  break;
  }


  my.AnimationPercent += animFactor * time_step;
  if (my.AnimationPercent > 100) my.AnimationPercent -= 100;
  ent_animate(me, animationName, my.AnimationPercent, animType);

  my.PreviousAction = my.CurrentAction;               //  Update previous action
}
