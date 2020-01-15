/************************************************************************
* hedit.c 	Hedit version 2.0 for Oasis OLC				*
* by Steve Wolfe - siv@cyberenet.net					*
 ************************************************************************/

#include "structs.h"
#include "comm.h"
#include "utils.h"
#include "db.h"
#include "olc.h"
#include "handler.h"

/* List each help entry saved, was used for debugging. */
#if 0
#define HEDIT_LIST	1
#endif

void hedit_setup_new(struct descriptor_data *d, char *new_key)
{
  CREATE(OLC_HELP(d), struct help_index_element, 1);

  OLC_HELP(d)->keywords = str_dup(new_key);
  OLC_HELP(d)->entry = str_dup("This is an unfinished help entry.\r\n");
  hedit_disp_menu(d);
  OLC_VAL(d) = 0;
}

/*------------------------------------------------------------------------*/

void hedit_setup_existing(struct descriptor_data *d, int rnum)
{
  struct help_index_element *help;

  /*
   * Build a copy of the help entry for editing.
   */
  CREATE(help, struct help_index_element, 1);

  *help = help_table[rnum];
  /*
   * Allocate space for all strings.
   */
  help->keywords = str_dup(help_table[rnum].keywords ?
    help_table[rnum].keywords : "UNDEFINED");
  help->entry = str_dup(help_table[rnum].entry ?
    help_table[rnum].entry : "undefined\r\n");

  /*
   * Attach copy of help entry to player's descriptor.
   */
  OLC_HELP(d) = help;
  OLC_VAL(d) = 0;
  hedit_disp_menu(d);
}

/*------------------------------------------------------------------------*/

void hedit_save_internally(struct descriptor_data *d)
{
  int i, rnum;
  struct help_index_element *new_help_table;

  rnum = OLC_ZNUM(d);
  /*
   * Help entry exists exists: free and replace it.
   */
  if (rnum > 0) {
    free_help(help_table + rnum);
    help_table[rnum] = *OLC_HELP(d);
  } else {			/* Entry doesn't exist, hafta add it. */
    CREATE(new_help_table, struct help_index_element, top_of_helpt + 2);

    /*
     * Insert new entry at the top - why not?
     */
    new_help_table[0] = *(OLC_HELP(d));

    /*
     * Count through help table.
     */
    for (i = 0; i <= top_of_helpt; i++)
      new_help_table[i + 1] = help_table[i];

    /*
     * Copy help table over to new one.
     */
    free(help_table);
    help_table = new_help_table;
    top_of_helpt++;
  }
  olc_add_to_save_list(HEDIT_PERMISSION, OLC_SAVE_HELP);
  update_help_index();
}

/*------------------------------------------------------------------------*/

void hedit_save_to_disk(void)
{
  int i;
  FILE *fp;
  struct help_index_element *help;

  sprintf(buf, "%s/%s.new", HLP_PREFIX, HELP_FILE);
  if (!(fp = fopen(buf, "w+"))) {
    mudlog("SYSERR: OLC: Cannot open help file!", BRF, LVL_IMMORT, TRUE);
    return;
  }
  for (i = 0; i <= top_of_helpt; i++) {
    help = (help_table + i);

#if defined(HEDIT_LIST)
    sprintf(buf1, "OLC: Saving help entry %d.", i);
    log(buf1);
#endif

    /*
     * Remove the '\r\n' sequences from description.
     */
    strcpy(buf1, help->entry ? help->entry : "Empty");
    strip_string(buf1);

    /*
     * Forget making a buffer, lets just write the thing now.
     */
    fprintf(fp, "%s\n%s%s#%d\n",
      help->keywords ? help->keywords : "UNDEFINED", buf1, (buf1[strlen(buf1) -1] == '\n') ? "" : "\n",
      help->min_level);
  }

  /*
   * Write final line and close.
   */
  fprintf(fp, "$~\n");
  fclose(fp);
  sprintf(buf2, "%s/%s", HLP_PREFIX, HELP_FILE);
  /*
   * We're fubar'd if we crash between the two lines below.
   */
  remove(buf2);
  rename(buf, buf2);

  olc_remove_from_save_list(HEDIT_PERMISSION, OLC_SAVE_HELP);
}

/*------------------------------------------------------------------------*/

void free_help(struct help_index_element *help)
{

  if (help->keywords)
    free(help->keywords);
  if (help->entry)
    free(help->entry);
}

/**************************************************************************
 Menu functions 
 **************************************************************************/

/*
 * The main menu.
 */
void hedit_disp_menu(struct descriptor_data *d)
{
  struct help_index_element *help;

  get_char_cols(d->character);
  help = OLC_HELP(d);

  sprintf(buf,
#if defined(CLEAR_SCREEN)
      "[H[J"
#endif
      "%s1%s) Keywords    : %s%s\r\n"
      "%s2%s) Entry       :\r\n%s%s"
      "%s3%s) Min Level   : %s%d\r\n"
      "%sQ%s) Quit\r\n"
      "Enter choice : ",

      grn, nrm, yel, help->keywords,
      grn, nrm, yel, help->entry,
      grn, nrm, cyn, help->min_level,
      grn, nrm
      );
  send_to_char(buf, d->character);

  OLC_MODE(d) = HEDIT_MAIN_MENU;
}

/**************************************************************************
  The main loop
 **************************************************************************/

void hedit_parse(struct descriptor_data *d, char *arg)
{
  int number;

  switch (OLC_MODE(d)) {
  case HEDIT_CONFIRM_SAVESTRING:
    switch (*arg) {
    case 'y':
    case 'Y':
      hedit_save_internally(d);
      sprintf(buf, "%s edits help for %s.", GET_NAME(d->character), OLC_HELP(d)->keywords);
      xlog(SYS_OLC, MAX(LVL_GOD, GET_INVIS_LEV(d->character)), buf, TRUE);
      /*
       * Do NOT free strings! Just the help structure. 
       */
      cleanup_olc(d, CLEANUP_STRUCTS);
      send_to_char("Help entry saved to memory.\r\n", d->character);
      break;
    case 'n':
    case 'N':
      /*
       * Free everything up, including strings, etc.
       */
      cleanup_olc(d, CLEANUP_ALL);
      break;
    default:
      send_to_char("Invalid choice!\r\nDo you wish to save this help entry internally? : ", d->character);
      break;
    }
    return;

  case HEDIT_MAIN_MENU:
    switch (*arg) {
    case 'q':
    case 'Q':
      if (OLC_VAL(d)) { /* Something has been modified. */
    send_to_char("Do you wish to save this help entry internally? : ", d->character);
    OLC_MODE(d) = HEDIT_CONFIRM_SAVESTRING;
      } else
    cleanup_olc(d, CLEANUP_ALL);
      return;
    case '1':
      send_to_char("Enter keywords:-\r\n] ", d->character);
      OLC_MODE(d) = HEDIT_KEYWORDS;
      break;
    case '2':
      OLC_MODE(d) = HEDIT_ENTRY;
#if defined(CLEAR_SCREEN)
      SEND_TO_Q("\x1B[H\x1B[J", d);
#endif
      SEND_TO_Q("Enter help entry: (/s saves /h for help)\r\n\r\n", d);
      d->backstr = NULL;
      if (OLC_HELP(d)->entry) {
    SEND_TO_Q(OLC_HELP(d)->entry, d);
    d->backstr = str_dup(OLC_HELP(d)->entry);
      }
      d->str = &OLC_HELP(d)->entry;
      d->max_str = MAX_HELP_ENTRY;
      d->mail_to[0] = 0;
      OLC_VAL(d) = 1;
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      break;
    case '3':
      send_to_char("Enter min level:-\r\n] ", d->character);
      OLC_MODE(d) = HEDIT_MIN_LEVEL;
      break;
    default:
      send_to_char("Invalid choice!\r\n", d->character);
      hedit_disp_menu(d);
      break;
    }
    return;

  case HEDIT_KEYWORDS:
    if (OLC_HELP(d)->keywords)
      free(OLC_HELP(d)->keywords);
    if (strlen(arg) > MAX_HELP_KEYWORDS)
      arg[MAX_HELP_KEYWORDS - 1] = '\0';
    OLC_HELP(d)->keywords = str_dup((arg && *arg) ? arg : "UNDEFINED");
    break;

  case HEDIT_ENTRY:
    /*
     * We will NEVER get here, we hope.
     */
    mudlog("SYSERR: Reached HEDIT_ENTRY case in parse_hedit", BRF, LVL_IMMORT, TRUE);
    break;

  case HEDIT_MIN_LEVEL:
    number = atoi(arg);
    if ((number < 0) || (number > LVL_IMPL))
      send_to_char("That is not a valid choice!\r\nEnter min level:-\r\n] ", d->character);
    else {
      OLC_HELP(d)->min_level = number;
      break;
    }
    return;

  default:
    /*
     * We should never get here.
     */
    mudlog("SYSERR: Reached default case in parse_hedit", BRF, LVL_IMMORT, TRUE);
    break;
  }
  /*
   * If we get this far, something has been changed.
   */
  OLC_VAL(d) = 1;
  hedit_disp_menu(d);
}

int find_help_rnum(char *keyword)
{
  struct help_indexer_element *el;
  
  if (!keyword || !*keyword) return -1;

  for (el = help_index;el;el = el->next)
    if (!strncasecmp(keyword, el->keyword, strlen(keyword)))
      return el->pos;
    
  return -1;
}

int hedit_delete_entry(int rnum) {
  int i, j = 0;
  struct help_index_element *new_help_table;

  if ((rnum < 0) || (rnum > top_of_helpt))
    return 0;

  CREATE(new_help_table, struct help_index_element, top_of_helpt);

  /*
   * Count through help table.
   */
  for (i = 0; i <= top_of_helpt; i++) {
    if (i != rnum) {
      new_help_table[j] = help_table[i];
      j++;
    } else {
      free_help(help_table + i);
    }
  }

  /*
   * Copy help table over to new one.
   */
  free(help_table);
  help_table = new_help_table;
  top_of_helpt--;
  olc_add_to_save_list(HEDIT_PERMISSION, OLC_SAVE_HELP);
  update_help_index();
  return 1;
}

ACMD(do_heditutil) {
  int i;
  struct descriptor_data *d;
  
  if (IS_NPC(ch) || !ch->desc) {
    send_to_char("Go away!\r\n", ch);
    return;
  }

  for (d = descriptor_list; d; d = d->next)
    if (d->connected == CON_HEDIT) {
      sprintf(buf, "Help files are already being editted by %s.\r\n",
              (CAN_SEE(ch, d->character) ? GET_NAME(d->character) : "someone"));
      send_to_char(buf, ch);
      return;
    }

  argument = one_word(argument, arg);
  skip_spaces(&argument);
  
  if (!arg || !*arg) {
    send_to_char("What do you wish to do to the help entries?\r\n", ch);
    return;
  }
  
  if (!strcasecmp(arg, "save")) {
    send_to_char("Saving all help information.\r\n", ch);
    sprintf(buf, "%s saves help info", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
    xlog(SYS_OLC, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
    hedit_save_to_disk();
    return;
  }
  
  if (!strcasecmp(arg, "delete")) {
    i = find_help_rnum(argument);
    if (i == -1) {
      send_to_char("Delete which help entry?\r\n", ch);
      return;
    }
    sprintf(buf, "Deleting \'%s\' from the help table.\r\n", help_table[i].keywords);
    send_to_char(buf, ch);
    sprintf(buf, "OLC: %s deletes \'%s\' from the help table", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), help_table[i].keywords);
    mudlog(buf, CMP, LVL_IMMORT, TRUE);
    hedit_delete_entry(i);
    return;
  }
  send_to_char("What do you wish to do to the help entries?\r\n", ch);
}
