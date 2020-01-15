/******************************************************************************
 * $Id: dynvars.c,v 1.13 2004/11/02 11:55:17 fang Exp $
 *
 * File: dynvars.c 
 * 
 * Created: Tue May 6 2003 by Stefan Lindstrom (asmodean@ludd.luth.se) 
 * 
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada,
 * yada...
 * 
 * Embedded "dynamic vars" in descriptions.
 *   
 *   $Log: dynvars.c,v $
 *   Revision 1.13  2004/11/02 11:55:17  fang
 *   Fixed dynvar bug with sticky return strings.
 *
 *   Revision 1.12  2003/10/10 07:37:50  fang
 *   Some more dynvars stuff
 *
 *   Revision 1.11  2003/08/18 10:44:24  fang
 *   Minor functionality to dynvars
 *
 *   Revision 1.10  2003/05/24 10:11:23  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2003/05/13 12:02:55  fang
 *   Minor corrections...
 *
 *   Revision 1.8  2003/05/12 10:30:27  fang
 *   All look_at_ functions in dynvars done.
 *
 *   Revision 1.7  2003/05/12 09:08:33  fang
 *   Enabled dynvars for room descs/xdescs
 *
 *   Revision 1.5  2003/05/10 14:59:27  fang
 *
 *   -
 *   More dynvars stuff. Left to do *should* be texts with both <?noformat and
 *   <?dynvars tags.
 *
 *   Revision 1.4  2003/05/10 10:04:20  fang
 *   More dynvars...
 *
 *   Revision 1.3  2003/05/08 19:27:16  fang
 *   More dynvars...
 *
 *   Revision 1.2  2003/05/08 13:18:16  fang
 *   Some more work on dyn.vars
 *
 *   Revision 1.1  2003/05/06 12:37:32  fang
 *   *** empty log message ***
 *
 *
 ******************************************************************************/
#include <ctype.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <strfunc.h>

#include "olc.h"
#include "db.h"
#include "dynvars.h"
#include "comm.h"
#include "utils.h"

static lua_State* luaVM = NULL;
static const char *begDPairRx = "(<\\?d)|(\\?>)"; 
static const char *begFPairRx =" (<\\?n)|(<\\?d)|(\\?>)";
static const char *beginNoFormat = "<?noformat";
static const char *beginDynVars  = "<?dynvars";
static const char *endTag        = "?>";

static void str_add(char **dest, const char *add)
{
  char *tmp;
  if (NULL == (*dest)) {
    *dest = strdup(add);
    return;
  }  
  tmp = calloc(strlen(*dest)+strlen(add)+1, sizeof(char));
  strcpy(tmp, (*dest));
  strcat(tmp, add);
  free(*dest);
  *dest = tmp;
} 

static char *dyn_remove_tags(const char *string)  {
  char *s1,*s2;
  replace_detach();
  s1 = replace((char *)string,(char *)beginNoFormat,"");
  replace_detach();
  s2 = replace((char *)s1  , (char *)endTag, "");
  replace_detach();
  free(s1);
  return s2;
}

static char *execute_code(const char *code)
{
  char buf[1024];
  const char *crc;
  int rc;

  if (NULL == luaVM) 
    return strdup("---[ embed code system not init'ed. notify an imm! ]---");

  rc = lua_dostring(luaVM,code);
  if (rc) {
    sprintf(buf,"---[ embed system failed to execute code. error %d. notify an imm! ]---",rc);
    return strdup(buf);
  }

  crc = lua_tostring(luaVM, -1);
  lua_pop(luaVM,1); 
  return strdup( (crc ? crc : " ") );
}

static char *remove_multi_ws_and_format(const char *rc, int width)
{
  svect *sv;
  int cnt = 0, i; 
  char *res;

  sv = sinit();
  splitf(sv, rc, NULL, 0);

  res = NULL;
  cnt = 0; 
  for (i = 0; i < sv->count; i++) {
     if (cnt + c_strlen(sv->list[i]) < width) {
       cnt += (c_strlen(sv->list[i])+1);
       str_add(&res,sv->list[i]);
       str_add(&res," ");
     } else {
       cnt = (c_strlen(sv->list[i])+3);
       str_add(&res,"\r\n");
       str_add(&res,sv->list[i]);
       str_add(&res," ");
     }
  }
  sfree(sv);

  return res;
}


/*
 * only format if contains vars!
 */ 
static char *dyn_autoformat(const char *str,int width)
{
  char *nf = strstr(str,beginNoFormat), *dv = strstr(str,beginDynVars), *res,*p;
  svect *sv = NULL,*sv2;
  int cnt;
  int i;
  char *rc = NULL;

  /*
   * no tags whatsoever!
   */
  if (NULL == nf && NULL == dv)
    return strdup(str);

  /*
   * dynamic vars, but no NoFormat-tags!
   */
  if (NULL != dv && NULL == nf) {
    sv = sinit();
    splitf(sv,str,begDPairRx, 4);

    for (i = 0; i < sv->count; i++) {
      if (strncmp(sv->list[i],"ynvars",6)) {
        replace_detach();
        rc = replace(sv->list[i],"\r\n"," ");
        replace_detach();
        free(sv->list[i]);
        sv->list[i] = rc;
        sv->lens[i] = strlen(rc);
        if (sv->lens[i] > sv->maxlen)
          sv->maxlen = sv->lens[i];
      } else {
         rc = execute_code(sv->list[i]+6); 
         free(sv->list[i]);
         sv->list[i] = rc;

         replace_detach();
         rc = replace(sv->list[i],"\r\n"," ");
         replace_detach();

         free(sv->list[i]);
         sv->list[i] = rc;
         sv->lens[i] = strlen(rc);
         if (sv->lens[i] > sv->maxlen)
           sv->maxlen = sv->lens[i];
      }
    }

    /* all string are ready to be smashed into one, single string... */
    p = sjoin(sv, " ");
    sfree(sv);

    /* remove multiple spaces from string */

    sv = sinit();
    splitf(sv, p, NULL, 0);

    res = NULL;
    cnt = 0; 
    for (i = 0; i < sv->count; i++) {
       if (cnt + c_strlen(sv->list[i]) < width) {
         cnt += (c_strlen(sv->list[i])+1);
         str_add(&res,sv->list[i]);
         str_add(&res," ");
       } else {
         cnt = (c_strlen(sv->list[i])+3);
         str_add(&res,"\r\n");
         str_add(&res,sv->list[i]);
         str_add(&res," ");
       }
    }
    sfree(sv);

    return res;
  }
  
  /*
   * Noformat tags, but no dynvars? Weirdo. ;)
   */
  if (NULL != nf && NULL == dv) {
    return dyn_remove_tags(str);
  }  

  /* 
   * if we get here, then we've got both dynvars AND noformat!
   */
  sv = sinit();
  splitf(sv,str,begFPairRx, 4);

  /*
   * First, just go through all code-pieces and execute them...
   */         
  for (i = 0; i < sv->count; i++) {
    if (!strncmp(sv->list[i],"ynvars",6)) {
      rc = execute_code(sv->list[i]+6);
      free(sv->list[i]);
      sv->list[i] = rc;
        
      replace_detach();
      rc = replace(sv->list[i],"\r\n"," ");
      replace_detach();
        
      free(sv->list[i]);
      sv->list[i] = rc;
      sv->lens[i] = strlen(rc); 
      if (sv->lens[i] > sv->maxlen)
        sv->maxlen = sv->lens[i];
    } else if (strncmp(sv->list[i],"ynvars",6) && strncmp(sv->list[i],"oformat",7)) {
      replace_detach(); 
      rc = replace(sv->list[i],"\r\n"," "); 
      replace_detach(); 
      free(sv->list[i]);
      sv->list[i] = rc;  
      sv->lens[i] = strlen(rc);
      if (sv->lens[i] > sv->maxlen)
        sv->maxlen = sv->lens[i];   
    }
  }

  /*
   * now merge, and autoformat all neighbouring pieces into singular slabs
   * of text. 
   */
  sv2 = sinit();
  rc = NULL;
  for (i = 0; i < sv->count; i++) {
    if (strncmp(sv->list[i],"oformat",7)) {
      str_add(&rc,sv->list[i]);
      str_add(&rc, " ");
    } else {
      if (rc) {
        p = remove_multi_ws_and_format(rc,width);
        free(rc);
        sadd(sv2,p);
        rc = NULL;
      }
      sadd(sv2,(sv->list[i]+7));
    }
  }
  if (rc) {
    p = remove_multi_ws_and_format(rc,width);
    free(rc);
    sadd(sv2,p);
    rc = NULL;
  }
  sfree(sv);
  sv = sv2;
  sv2 = NULL;

  rc = sjoin(sv, " ");
  sfree(sv);

  return strdup(rc);
}

static void push_lua_vars(character_t *ch, character_t *vict, room_t *rm, obj_t *obj, zone_t *z)
{
  if (NULL != luaVM) {
    /* push char as global var */
    lua_pushstring(luaVM,"ch");
    lua_pushlightuserdata(luaVM, ch);
    lua_settable(luaVM,LUA_GLOBALSINDEX);

    /* push victim as global var */
    lua_pushstring(luaVM,"victim");
    lua_pushlightuserdata(luaVM, vict);
    lua_settable(luaVM,LUA_GLOBALSINDEX);

    /* push room as global var */
    lua_pushstring(luaVM,"room");
    lua_pushlightuserdata(luaVM, rm);
    lua_settable(luaVM,LUA_GLOBALSINDEX);

    /* push object as global var */
    lua_pushstring(luaVM,"obj");
    lua_pushlightuserdata(luaVM, obj);
    lua_settable(luaVM,LUA_GLOBALSINDEX);

    /* push zone as global var */
    lua_pushstring(luaVM,"zone");
    lua_pushlightuserdata(luaVM, z);
    lua_settable(luaVM,LUA_GLOBALSINDEX);
  }
}

static int dyn_gecho(lua_State* luaVM)
{
  char *toEcho = (char *)lua_tostring(luaVM, -1);
  send_to_all(toEcho);
  return 0;
}

static int dyn_charname(lua_State* luaVM)
{ 
  characterptr_t ch = (characterptr_t)lua_topointer(luaVM, -1);
  lua_pushstring(luaVM, GET_NAME(ch));
  return 1;
}

/*
 * called: echo_to_char(str, ch);
 */
static int dyn_sendtochar(lua_State* luaVM)
{ 
  characterptr_t ch = (characterptr_t)lua_topointer(luaVM, -2);
  char *toEcho = (char *)lua_tostring(luaVM, -1);
  send_to_char(toEcho, ch);
  return 0;
}

static int dyn_hsenses(lua_State* luaVM)
{
  character_t *ch = (character_t *)lua_topointer(luaVM, -1);

  if (GET_WOLFKIN(ch) || PRF_FLAGGED(ch,PRF_GRASPING) || PRF_FLAGGED(ch,PRF_TGRASP))
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

 if (GET_WOLFKIN(ch)) 
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

 if (PRF_FLAGGED(ch,PRF_GRASPING))
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

 if (PRF_FLAGGED(ch,PRF_TGRASP))
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

  return 4;
}

/*
 * s, r = sun();
 * s = true if any form of sun. 
 * r = true if SUN_RISING
 */ 
static int dyn_sun(lua_State *luaVM)
{
  if (SUN_RISE == weather_info.sunlight || SUN_LIGHT == weather_info.sunlight)
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

  if (SUN_RISE == weather_info.sunlight)
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

  return 2;
} 

/*
 * n, s = nosun();
 * n = true if no form of sun.
 * s = true if SUN_SET
 */
static int dyn_nosun(lua_State *luaVM)
{
  if (SUN_SET == weather_info.sunlight || SUN_DARK == weather_info.sunlight)
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);

  if (SUN_SET == weather_info.sunlight)
    lua_pushboolean(luaVM, 1);
  else
    lua_pushboolean(luaVM, 0);
  
  return 2;
}

static int dyn_hour(lua_State *luaVM)
{
  lua_pushnumber(luaVM,time_info.hours);
  return 1;
}

static int dyn_cloudless(lua_State *luaVM) 
{
  if (SKY_CLOUDLESS == weather_info.sky) 
    lua_pushboolean(luaVM,1);
  else
    lua_pushboolean(luaVM,0);  
  
  return 1;
}

static int dyn_clouded(lua_State *luaVM)
{
  if (SKY_CLOUDY == weather_info.sky)
    lua_pushboolean(luaVM,1);
  else
    lua_pushboolean(luaVM,0);

  return 1;
}

static int dyn_raining(lua_State *luaVM)
{ 
  if (SKY_RAINING == weather_info.sky) 
    lua_pushboolean(luaVM,1);
  else
    lua_pushboolean(luaVM,0);  

  return 1;
}

static int dyn_lightning(lua_State *luaVM)
{ 
  if (SKY_LIGHTNING == weather_info.sky) 
    lua_pushboolean(luaVM,1);
  else
    lua_pushboolean(luaVM,0);  
  
  return 1;
}

static int dyn_random(lua_State *luaVM)
{
  int lo = lua_tonumber(luaVM, -2);
  int hi = lua_tonumber(luaVM, -1);
	
  int rl, rh;

  rl = MIN(lo, hi);
  rh = MAX(lo, hi); 
  
  alog("(lo,hi) = (%d,%d), (rl, rh) = (%d,%d)", lo, hi, rl, rh);

  lua_pushnumber(luaVM, number(rl, rh));

  return 1;
}

static int dyn_channeler(lua_State *luaVM)
{
  struct char_data *ch = (characterptr_t)lua_topointer(luaVM, -1);

  lua_pushboolean(luaVM, (cant_channel(ch) ? 0 : 1));

  return 1;
}

static int dyn_male(lua_State *luaVM)
{
  struct char_data *ch = (characterptr_t)lua_topointer(luaVM, -1);

  lua_pushboolean(luaVM, (SEX_MALE == GET_SEX(ch) ? 1 : 0));

  return 1;
}

static int dyn_female(lua_State *luaVM)
{
  struct char_data *ch = (characterptr_t)lua_topointer(luaVM, -1);

  lua_pushboolean(luaVM, (SEX_FEMALE == GET_SEX(ch) ? 1 : 0));

  return 1;
}

static int dyn_nogender(lua_State *luaVM)
{
  struct char_data *ch = (characterptr_t)lua_topointer(luaVM, -1);

  lua_pushboolean(luaVM, (SEX_NEUTRAL == GET_SEX(ch) ? 1 : 0));

  return 1;
}

static int dyn_dice(lua_State *luaVM)
{
  int numdice = lua_tonumber(luaVM, -3);
  int sides   = lua_tonumber(luaVM, -2);
  int add     = lua_tonumber(luaVM, -1);

  lua_pushnumber( luaVM, (dice(numdice, sides)+add) );

  return 1;
}

static int dyn_poisson(lua_State *luaVM) {
  double mean = lua_tonumber(luaVM, -1);
  int res = randPoisson(mean);

  lua_pushnumber(luaVM, res);

  return 1;
}

int init_dynvars(void)
{
  if (NULL != luaVM)
    return 1; // already inited!

  if (NULL == (luaVM = lua_open()))
    return 0;

  lua_baselibopen(luaVM);
  lua_iolibopen(luaVM);
  lua_strlibopen(luaVM);
  lua_mathlibopen(luaVM);

  lua_register(luaVM, "hsenses"     , dyn_hsenses);
  lua_register(luaVM, "gecho"       , dyn_gecho);
  lua_register(luaVM, "chname"      , dyn_charname);
  lua_register(luaVM, "echo_to_char", dyn_sendtochar);
  lua_register(luaVM, "sun"         , dyn_sun);
  lua_register(luaVM, "nosun"       , dyn_nosun);
  lua_register(luaVM, "hour"        , dyn_hour);
  lua_register(luaVM, "clouded"     , dyn_clouded);
  lua_register(luaVM, "cloudless"   , dyn_cloudless);
  lua_register(luaVM, "raining"     , dyn_raining);
  lua_register(luaVM, "lightning"   , dyn_lightning);
  lua_register(luaVM, "dice"        , dyn_dice);
  lua_register(luaVM, "random"      , dyn_random);
  lua_register(luaVM, "channeler"   , dyn_channeler);
  lua_register(luaVM, "male"        , dyn_male);
  lua_register(luaVM, "female"      , dyn_female);
  lua_register(luaVM, "nogender"    , dyn_nogender);
  lua_register(luaVM, "poisson"     , dyn_poisson);

  return 1;
}

int close_dynvars(void)
{
  lua_close(luaVM);
  luaVM = NULL;

  return 1;
}

char *char_look_at_room(character_t *ch, const char *desc)
{
  char *ptr;
  push_lua_vars(ch, NULL, &world[ch->in_room],NULL,&zone_table[real_zone(world[ch->in_room].number)]);

  ptr = dyn_autoformat(desc,FORMAT_WIDTH);
  if ('\n' != *(ptr+strlen(ptr) - 1))
    str_add(&ptr,"\r\n");
  return ptr;
}

char *char_look_at_obj(character_t *ch, obj_t *obj, const char *desc)
{
  char *ptr;
  push_lua_vars(ch, NULL, &world[ch->in_room], obj,&zone_table[real_zone(world[ch->in_room].number)]);
  
  ptr = dyn_autoformat(desc,FORMAT_WIDTH);
  if ('\n' != *(ptr+strlen(ptr) - 1))
    str_add(&ptr,"\r\n");
  return ptr;
}

char *char_look_at_char(character_t *ch, character_t *vict, const char *desc)
{ 
  char *ptr;
  push_lua_vars(ch,vict, &world[ch->in_room],NULL,&zone_table[real_zone(world[ch->in_room].number)]);
  ptr = dyn_autoformat(desc,FORMAT_WIDTH);
  
  if ('\n' != *(ptr+strlen(ptr) - 1))
    str_add(&ptr,"\r\n");
  return ptr;
}

