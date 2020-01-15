/******************************************************************************
 * $Id: act.h,v 1.22 2003/06/17 10:30:05 fang Exp $
 *   
 * File: act.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.h,v $
 *   Revision 1.22  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.21  2003/06/12 07:22:13  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.20  2003/05/22 02:18:40  fang
 *   The zap command (do_pzap) was only useful for harassing morts. By admin
 *   request, it has been disabled. Instead of removing it, I just commented
 *   it out. --Tze
 *
 *   Revision 1.19  2003/05/11 14:50:20  fang
 *   DYnvar/noformat handling code should be
 *    complete. Minor bugs may remain, so keep an eye open
 *
 *   Revision 1.18  2003/03/04 22:06:04  fang
 *   fisch: added a quick logger function to add coments to syslog
 *
 *   Revision 1.17  2003/03/01 07:17:04  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.16  2003/02/03 09:04:44  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.15  2002/10/31 03:01:03  fang
 *   fischer: removing the last traces of old newbiegoto
 *
 *   Revision 1.14  2002/10/29 03:09:26  fang
 *   fisch: added the command "scene".  Scenes are like doings for rooms.
 *
 *   Revision 1.13  2002/10/23 04:17:19  fang
 *   fisch: "list board" to list boards with new messages added
 *
 *   Revision 1.12  2002/10/22 04:14:59  fang
 *   fisch: removing gboard
 *
 *   Revision 1.11  2002/10/22 03:31:46  fang
 *   Fisch: added code for zcmd command
 *
 *   Revision 1.10  2002/10/16 16:54:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/10/14 03:15:50  fang
 *   fisch: added "edit" for the message board messages
 *
 *   Revision 1.8  2002/10/09 19:13:01  fang
 *   Added "apropos" command to search help texts.
 *
 *   Revision 1.7  2002/09/12 22:21:33  fang
 *   fischer: removed the prove and journal commands
 *
 *   Revision 1.6  2002/09/10 18:37:53  fang
 *   Drath: Added HELPCHECK and HINDEX
 *
 *   Revision 1.5  2002/09/08 06:41:16  fang
 *   Drath: Fischer added AEDIT.  Welcome to the staff, Fischer.
 *
 *   Revision 1.4  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.3  2002/08/12 21:08:20  fang
 *   Drath: PLIST added
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.15  2002/07/16 15:16:16  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.14  2002/07/12 03:37:33  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.13  2002/07/09 22:37:17  fang
 *   Drath: REPLYLOCK
 *
 *   Revision 1.12  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.11  2002/06/29 18:33:04  fang
 *   Drath: In-MUD syslog searching.  Replaces websyslog search
 *
 *   Revision 1.10  2002/06/23 19:11:18  fang
 *   Drath: Fix to do_echo and first commit of XLOG
 *
 *   Revision 1.9  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.8  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.7  2002/05/28 20:22:28  fang
 *   Maz: Small fix for Nose/Ear Stat Removal
 *
 *   Revision 1.6  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.5  2002/05/06 02:20:20  fang
 *   Mazrim: fixed Ajahset to check off guild 53
 *
 *   Revision 1.4  2002/05/01 13:02:18  fang
 *   Added: Cleartells.  Silly command that no one will use.  But it's there now.
 *
 *   Revision 1.3  2002/03/08 10:34:59  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:27  fang
 *   *** empty log message ***
 *
 *   Revision 1.53  2002/01/31 15:17:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.52  2002/01/31 15:03:59  mud
 *   Added hlist for Mas.
 *
 *   Revision 1.51  2002/01/30 09:20:43  mud
 *   shortgate added. Also made gate not choke on unknown rooms, if distance between rooms are not morre than 10 rooms
 *
 *   Revision 1.50  2002/01/28 15:29:04  mud
 *   Customizable prompts..
 *
 *   Revision 1.49  2002/01/28 08:52:56  mud
 *   Changed delete so that is a character has qps (raw, skills or eq)
 *   the normal delete proicedure will not work, and a warning is printed.
 *   Instead of "delete yes delete me" (qhich still works for no-qp chars)
 *   the qp-chars will have to type "delete yes delete me and my qps!".
 *
 *   Revision 1.48  2002/01/24 21:33:11  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.47.2.7  2002/01/24 20:49:36  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.47.2.6  2002/01/24 03:20:28  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.47.2.5  2002/01/23 21:49:36  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.47.2.4  2002/01/22 03:05:08  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.47.2.3  2002/01/21 01:23:37  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.47.2.2  2002/01/18 02:24:50  mud
 *   More cleanup...
 *
 *   Revision 1.47.2.1  2002/01/18 00:44:19  mud
 *   Cleanup started for real. Removed ct, ctalk, dt, tt, wt, ot and a few other leftovers from old guild system.
 *
 *   Revision 1.41  2001/05/22 09:07:01  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.37  2001/04/25 08:50:57  mud
 *   Email and background entry added to pfiles, commands for setting email/background, and a viewbground command added.
 *
 *   Revision 1.36  2001/04/21 02:56:06  mud
 *   QPadd and GequipSend, plus greceive
 *
 *   Revision 1.3  2000/03/01 11:48:00  mud
 *   Rcopy function added.
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __ACT_H__
#define __ACT_H__

#include "structs.h"
#include "interpreter.h"

/*
 * Data
 */

/*
 * Funcs
 */
extern void do_newbie_eq(struct char_data *ch);
extern int list_obj_to_char(struct obj_data * list, struct char_data * ch, int mode, bool show);
extern int mastery(struct char_data *ch);
extern void diag_char_to_char(struct char_data * i, struct char_data * ch);
extern void list_char_to_char(struct char_data * list, struct char_data * ch);
extern void remove_defense(struct char_data *ch);
extern void do_defense(struct char_data *ch);
extern void wakeup_sequence(struct char_data *ch);
extern void perform_immort_vis(struct char_data *ch);
extern struct affected_type clear_aff_var(struct affected_type af);
extern int find_eq_pos(struct char_data * ch, struct obj_data * obj,char *arg);
extern int check_weapon_alias(struct char_data *ch, int mastery_type);
extern char *fread_action(FILE * fl, int nr);
extern void sort_commands(void);
extern void boot_social_messages(void);
extern void create_command_list(void);
extern void perform_wear(struct char_data * ch, struct obj_data * obj, int where);
extern int  put_obj_on_obj(struct obj_data *to_put,struct obj_data *put_on);
extern int  char_sit_on_obj(struct char_data *ch, struct obj_data *obj);
extern int  char_sleep_on_obj(struct char_data *ch, struct obj_data *obj);
extern void add_tell_to_buffer(struct char_data *ch, char buf[MAX_INPUT_LENGTH]);
extern void add_to_rplog(struct char_data *ch, char *buf, struct char_data *from);
extern sh_int find_target_room(struct char_data * ch, char *rawroomstr);
extern void perform_remove(struct char_data * ch, int pos);
extern void get_qs(struct char_data *ch, long *inskill, long *ingear);

/*
 * ACMDs
 */
extern ACMD(do_knifetoss);
extern ACMD(do_markedmap);
extern ACMD(do_eavesdrop);
extern ACMD(do_bargain);
extern ACMD(do_become);
extern ACMD(do_tedit);
extern ACMD(do_private_channel);
extern ACMD(do_prefix);
extern ACMD(do_seekerauthorise);
extern ACMD(do_zecho);
extern ACMD(do_manaconv);
extern ACMD(do_wlist);
extern ACMD(do_preach);
extern ACMD(do_terinfo);
extern ACMD(do_tertest);
extern ACMD(do_pemote);
extern ACMD(do_pose);
extern ACMD(do_return);
extern ACMD(do_say);
extern ACMD(martial_arts);
extern ACMD(do_drop);
extern ACMD(do_get);
extern ACMD(do_dc);
extern ACMD(do_split);
extern ACMD(do_flee);
extern ACMD(do_disarm);
extern ACMD(do_stun);  
extern ACMD(do_follow);
extern ACMD(do_dream);
extern ACMD(do_kick);
extern ACMD(do_punch);
extern ACMD(do_dirtkick);
extern ACMD(do_trip);
extern ACMD(do_bash);
extern ACMD(do_action);
extern ACMD(do_awareness);
extern ACMD(do_look);
extern ACMD(do_grasp);
extern ACMD(do_cast);
extern ACMD(do_charm);
extern ACMD(do_delete);
extern ACMD(do_dismount);
extern ACMD(do_gencart);
extern ACMD(do_speedwalk);
extern ACMD(do_adt);    
extern ACMD(do_advance);
extern ACMD(do_aedit);
extern ACMD(do_affected);  
extern ACMD(do_afk);      
extern ACMD(do_afw);      
extern ACMD(do_aim);
extern ACMD(do_ajahset);
extern ACMD(do_alias);
extern ACMD(do_amt);    
extern ACMD(do_apply); 
extern ACMD(do_appraise);
extern ACMD(do_apropos);
extern ACMD(do_armscry); 
extern ACMD(do_arrest);   
extern ACMD(do_arrogance);
extern ACMD(do_assist);   
extern ACMD(do_at);
extern ACMD(do_authorize);
extern ACMD(do_unauthorize);
extern ACMD(do_award);
extern ACMD(do_background);
extern ACMD(do_backstab);
extern ACMD(do_ban);
extern ACMD(do_bandmarch);
extern ACMD(do_bdt);
extern ACMD(do_bmlist);
extern ACMD(do_multilist);
extern ACMD(bondsense);
extern ACMD(do_brand); 
extern ACMD(do_breakshield);
extern ACMD(do_bugcheck);   
extern ACMD(do_call);
extern ACMD(do_camo);
extern ACMD(do_camp);
extern ACMD(do_cemote);
extern ACMD(do_charge);
extern ACMD(do_clearaff);
extern ACMD(do_clearbond);
extern ACMD(do_color);
extern ACMD(do_commands);
extern ACMD(do_compare); 
extern ACMD(do_consider);
extern ACMD(do_craft);   
extern ACMD(do_credits); 
extern ACMD(do_ctell);
extern ACMD(do_darkdream);
extern ACMD(do_darksay);  
extern ACMD(do_date);
extern ACMD(do_dazzle);     
extern ACMD(do_desc);     
extern ACMD(do_dig);      
extern ACMD(do_doing);    
extern ACMD(do_scene);    
extern ACMD(do_drag);     
extern ACMD(do_dreamlist);
extern ACMD(do_dreampull);
extern ACMD(do_dset);
extern ACMD(do_concentrate);
extern ACMD(do_dt);
extern ACMD(do_diagnose);
extern ACMD(do_dice);
extern ACMD(do_disengage);
extern ACMD(do_display);  
extern ACMD(do_dns);
extern ACMD(do_dist_spell);
extern ACMD(do_drink);
extern ACMD(do_clone);
extern ACMD(do_eat);  
extern ACMD(do_echo); 
extern ACMD(do_employ);
extern ACMD(do_enhance);
extern ACMD(do_enter);  
extern ACMD(do_equipment);
extern ACMD(do_examine);  
extern ACMD(do_exit);     
extern ACMD(do_exits);    
extern ACMD(do_fervor);   
extern ACMD(do_free);     
extern ACMD(do_findherb); 
extern ACMD(do_fine);
extern ACMD(do_fire);
extern ACMD(do_flank);
extern ACMD(do_forage);
extern ACMD(do_ginvis);
extern ACMD(do_subdue);
extern ACMD(do_supply);
extern ACMD(do_force); 
extern ACMD(do_forget);
extern ACMD(do_fortify);
extern ACMD(do_fx);
extern ACMD(do_gdt);
extern ACMD(do_gecho);
extern ACMD(do_gen_comm);
extern ACMD(do_gen_door);
extern ACMD(do_gen_ps);  
extern ACMD(do_gen_tog); 
extern ACMD(do_gen_write);
extern ACMD(do_getaride); 
extern ACMD(do_give);     
extern ACMD(do_glance);   
extern ACMD(do_glist);    
extern ACMD(do_gold);     
extern ACMD(do_goto);     
extern ACMD(do_guild);    
extern ACMD(do_grab);
extern ACMD(do_group);
extern ACMD(do_gsay); 
extern ACMD(do_gset); 
extern ACMD(do_hallow);
extern ACMD(do_handtalk);
extern ACMD(do_help);
extern ACMD(do_helpcheck);
extern ACMD(do_hindex);
extern ACMD(do_newpolicy);
extern ACMD(do_policy);
extern ACMD(do_policies);
extern ACMD(do_herblore);
extern ACMD(do_herbmastery);
extern ACMD(do_hgive);
extern ACMD(do_hide); 
extern ACMD(do_hide_tracks);
extern ACMD(do_hire);
extern ACMD(do_hit); 
extern ACMD(do_hunt);
extern ACMD(do_home);
extern ACMD(do_horseorder);
extern ACMD(do_horsedesc); 
extern ACMD(do_horseinv);  
extern ACMD(do_horselist); 
extern ACMD(do_horsename); 
extern ACMD(do_horsetrain);
extern ACMD(do_imagine);   
extern ACMD(do_immlock);
extern ACMD(do_info);      
extern ACMD(do_inspire);   
extern ACMD(do_instruct);  
extern ACMD(do_insult);    
extern ACMD(do_inventory); 
extern ACMD(do_inquisition);
extern ACMD(do_invis);
extern ACMD(do_isic); 
extern ACMD(do_multi);
extern ACMD(do_authcheck);
extern ACMD(do_dreamgoto);
extern ACMD(do_kill);
extern ACMD(do_knock);
extern ACMD(do_knowledge);
extern ACMD(do_know_health);
extern ACMD(do_language);   
extern ACMD(do_log);
extern ACMD(do_logsearch);
extern ACMD(do_last);
extern ACMD(do_lastemote);
extern ACMD(do_learn);
extern ACMD(do_leave);
extern ACMD(do_lines);
extern ACMD(do_link); 
extern ACMD(do_lipreading);
extern ACMD(do_load);
extern ACMD(do_locate);
extern ACMD(do_mask);  
extern ACMD(do_mdt);   
extern ACMD(do_mediate);
extern ACMD(do_medicine);
extern ACMD(do_morale);  
extern ACMD(do_move);    
extern ACMD(do_mount);   
extern ACMD(do_blademastery);
extern ACMD(do_navigate);
extern ACMD(do_newbiegoto);
extern ACMD(do_not_here);
extern ACMD(do_noot);
extern ACMD(do_notes);   
extern ACMD(do_noafk);   
extern ACMD(do_noafw);   
extern ACMD(do_nosnoop); 
extern ACMD(do_obscure); 
extern ACMD(do_offer);   
extern ACMD(do_offend);  
extern ACMD(do_olc);     
extern ACMD(do_order);   
extern ACMD(do_osay);
extern ACMD(do_pack);    
extern ACMD(do_page);    
extern ACMD(do_passwd);  
extern ACMD(do_play);    
extern ACMD(do_playerlist);
extern ACMD(do_poofset);   
extern ACMD(do_pound);     
extern ACMD(do_pour);      
extern ACMD(do_powertrap); 
extern ACMD(do_practice);  
extern ACMD(do_privacy);   
extern ACMD(do_prodigy);   
extern ACMD(do_pull);      
extern ACMD(dont_pull);    
extern ACMD(do_purge);     
extern ACMD(do_push);      
extern ACMD(do_put);       
extern ACMD(do_qcomm);     
extern ACMD(do_qlist);     
extern ACMD(do_qstat);     
extern ACMD(do_quit);      
extern ACMD(do_rcopy);
extern ACMD(do_revive);
extern ACMD(do_rage);      
extern ACMD(do_ranks);     
extern ACMD(do_reboot);    
extern ACMD(do_recall);    
extern ACMD(do_recruit);   
extern ACMD(do_refresh);   
extern ACMD(do_reinforce); 
extern ACMD(do_release);   
extern ACMD(do_remove);    
extern ACMD(do_rent);      
extern ACMD(do_repair);    
extern ACMD(do_reply);     
extern ACMD(do_replylock);     
extern ACMD(do_report);    
extern ACMD(do_oreport);   
extern ACMD(do_rescue);    
extern ACMD(do_rest);      
extern ACMD(do_restore);   
extern ACMD(do_retreat);   
extern ACMD(do_roomset);
extern ACMD(do_rumor);     
extern ACMD(do_run);       
extern ACMD(do_save);      
extern ACMD(do_saltbrothers);
extern ACMD(do_sayto);     
extern ACMD(do_scan);      
extern ACMD(do_score);     
extern ACMD(do_search);    
extern ACMD(do_seek);      
extern ACMD(do_seeker);    
extern ACMD(do_select_quest);
extern ACMD(do_send);
extern ACMD(do_set); 
extern ACMD(do_sever);
extern ACMD(do_shieldwall);
extern ACMD(do_shoot);
extern ACMD(do_show); 
extern ACMD(do_shutdown);
extern ACMD(do_sit);
extern ACMD(do_skills);
extern ACMD(do_skillset);
extern ACMD(do_sleep);   
extern ACMD(do_sneak);   
extern ACMD(do_snoop);   
extern ACMD(do_speardance);
extern ACMD(do_spec_comm); 
extern ACMD(do_spells);    
extern ACMD(do_sponsor);
extern ACMD(do_sponsorlist);
extern ACMD(do_stab);      
extern ACMD(do_stalk);     
extern ACMD(do_stand);     
extern ACMD(do_stat);      
extern ACMD(do_steal);     
extern ACMD(do_subguild);  
extern ACMD(do_subrank);
extern ACMD(do_rsubrank);   
extern ACMD(do_summon);
extern ACMD(do_survival);
extern ACMD(do_shift);   
extern ACMD(do_switch);  
extern ACMD(do_syslog);  
extern ACMD(do_tail);    
extern ACMD(do_teach);   
extern ACMD(do_teleport);
extern ACMD(do_tell);
extern ACMD(do_test);
extern ACMD(do_throw);
extern ACMD(do_tie);  
extern ACMD(do_time); 
extern ACMD(do_title);
extern ACMD(do_toggle);
extern ACMD(do_tool);  
extern ACMD(do_track); 
extern ACMD(do_tract); 
extern ACMD(do_trans); 
extern ACMD(do_travel);
extern ACMD(do_treesing);
extern ACMD(do_unaffect);
extern ACMD(do_unban);   
extern ACMD(do_unbond);  
extern ACMD(do_unpack);  
extern ACMD(do_ungroup); 
extern ACMD(do_unsever);
extern ACMD(do_untaint); 
extern ACMD(do_use);
extern ACMD(do_users);
extern ACMD(do_veil); 
extern ACMD(do_visible);
extern ACMD(do_vnum);   
extern ACMD(do_void);   
extern ACMD(do_vstat);  
extern ACMD(do_wait);   
extern ACMD(do_wake);   
extern ACMD(do_wash);   
extern ACMD(do_wear);   
extern ACMD(do_weather);
extern ACMD(do_where);  
extern ACMD(do_who);    
extern ACMD(do_whois);  
extern ACMD(do_wield);  
extern ACMD(do_wimpy);  
extern ACMD(do_wizlock);
extern ACMD(do_wiznet); 
extern ACMD(do_wizutil);
extern ACMD(do_worth);  
extern ACMD(do_write);  
extern ACMD(do_wkname); 
extern ACMD(do_xname);
extern ACMD(do_xlog);
extern ACMD(do_zap);
extern ACMD(do_zlock);
extern ACMD(do_zreset);
extern ACMD(t_grasp);  
extern ACMD(t_release);
extern ACMD(do_infravision);
extern ACMD(do_questlock);  
extern ACMD(do_dreamenter); 
extern ACMD(do_timelock);
extern ACMD(do_isquest);
extern ACMD(do_receive);
extern ACMD(do_check);
extern ACMD(do_mail);
extern ACMD(do_passbond); 
extern ACMD(do_longsleep);
extern ACMD(do_gossip);
extern ACMD(do_pray);
extern ACMD(do_herbident);
extern ACMD(do_sponsorset);
extern ACMD(do_addguild);
extern ACMD(do_remguild);
extern ACMD(do_newremguild);
extern ACMD(do_allguilds);
extern ACMD(do_clubs);
extern ACMD(do_myguilds);
extern ACMD(do_guildinfo);
extern ACMD(do_guildset);
extern ACMD(do_seekguild);
extern ACMD(do_guildieset);
extern ACMD(do_seeking);
extern ACMD(do_deguild);
extern ACMD(do_gdesc);
extern ACMD(do_greq);
extern ACMD(do_gadmininfo);
extern ACMD(do_saveguild);
extern ACMD(do_gdeposit);
extern ACMD(do_gwithdraw);
extern ACMD(do_gskillset);
extern ACMD(do_guildieinfo);
extern ACMD(do_gload);
extern ACMD(do_granks);
extern ACMD(do_newdisguise);
extern ACMD(do_guilddisguise);
extern ACMD(do_newghelp);
extern ACMD(do_twt);
extern ACMD(do_tast);
extern ACMD(do_heditsave);
extern ACMD(do_heditutil);
extern ACMD(do_peditutil);
extern ACMD(do_cleartells);
extern ACMD(do_tells);
extern ACMD(do_rplog);
extern ACMD(do_wolfcall);
extern ACMD(do_wolfsense);
extern ACMD(do_lweapon);
extern ACMD(do_qcount);
extern ACMD(do_showmail); /* mail.c */
extern ACMD(do_unsponsor);
extern ACMD(do_qlist);
extern ACMD(do_inform);
/* extern ACMD(do_ignorelist); */
extern ACMD(do_notifylist);
extern ACMD(do_greceive);
extern ACMD(do_email);
extern ACMD(do_contactinfo);
extern ACMD(do_bground);
extern ACMD(do_showbground);
extern ACMD(do_dswitch);
/* extern ACMD(do_pzap); */

/* DG Script ACMD's */
extern ACMD(do_attach);
extern ACMD(do_detach);
extern ACMD(do_tlist);
extern ACMD(do_tstat);
extern ACMD(do_masound);
extern ACMD(do_mkill);
extern ACMD(do_mjunk);
extern ACMD(do_mdoor);
extern ACMD(do_mechoaround);
extern ACMD(do_msend);
extern ACMD(do_mecho);
extern ACMD(do_mload);
extern ACMD(do_mpurge);
extern ACMD(do_mgoto);
extern ACMD(do_mat);
extern ACMD(do_mteleport);
extern ACMD(do_mforce);
extern ACMD(do_mexp);
extern ACMD(do_mgold);
extern ACMD(do_mhunt);
extern ACMD(do_mremember);
extern ACMD(do_mforget);
extern ACMD(do_mtransform);
extern ACMD(do_vdelete);
extern ACMD(do_dgstat);
extern ACMD(do_prompt);
extern ACMD(do_shortgate);
extern ACMD(do_hlist);
extern ACMD(do_plist);
extern ACMD(do_rlist);
extern ACMD(do_mycolor);
extern ACMD(do_edit);
extern ACMD(do_list);
extern ACMD(do_zcmd);

extern void de_boot_socials(void);
extern int find_multi( struct char_data *ch, int *iImm, int *iMort, int *iUnauth );

#endif
