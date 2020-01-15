/************************************************************************
* pedit.c 	Pedit version 2.0 for Oasis OLC				*
*
* POLICY editor for Dragons Fang.  Modified version of
* HEdit 2.0 for Oasis OLC
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
#define PEDIT_LIST	1
#endif

void pedit_setup_new(struct descriptor_data *d, char *new_key)
{
  CREATE(OLC_POLI(d), struct poli_index_element, 1);

  OLC_POLI(d)->keywords = str_dup(new_key);
  OLC_POLI(d)->entry = str_dup("This is an unfinished policy entry.\r\n");
  pedit_disp_menu(d);
  OLC_VAL(d) = 0;
}

/*------------------------------------------------------------------------*/

void pedit_setup_existing(struct descriptor_data *d, int rnum)
{
  struct poli_index_element *poli;

  /*
   * Build a copy of the help entry for editing.
   */
  CREATE(poli, struct poli_index_element, 1);

  *poli = poli_table[rnum];
  /*
   * Allocate space for all strings.
   */
  poli->keywords = str_dup(poli_table[rnum].keywords ?
    poli_table[rnum].keywords : "UNDEFINED");
  poli->entry = str_dup(poli_table[rnum].entry ?
    poli_table[rnum].entry : "undefined\r\n");

  /*
   * Attach copy of help entry to player's descriptor.
   */
  OLC_POLI(d) = poli;
  OLC_VAL(d) = 0;
  pedit_disp_menu(d);
}

/*------------------------------------------------------------------------*/

void pedit_save_internally(struct descriptor_data *d)
{
  int i, rnum;
  struct poli_index_element *new_poli_table;

  rnum = OLC_ZNUM(d);
  /*
   * Help entry exists exists: free and replace it.
   */
  if (rnum > 0) {
    free_poli(poli_table + rnum);
    poli_table[rnum] = *OLC_POLI(d);
  } else {			/* Entry doesn't exist, hafta add it. */
    CREATE(new_poli_table, struct poli_index_element, top_of_polit + 2);

    /*
     * Insert new entry at the top - why not?
     */
    new_poli_table[0] = *(OLC_POLI(d));

    /*
     * Count through help table.
     */
    for (i = 0; i <= top_of_polit; i++)
      new_poli_table[i + 1] = poli_table[i];

    /*
     * Copy help table over to new one.
     */
    free(poli_table);
    poli_table = new_poli_table;
    top_of_polit++;
  }
  olc_add_to_save_list(PEDIT_PERMISSION, OLC_SAVE_POLI);
  update_poli_index();
}

/*------------------------------------------------------------------------*/

void pedit_save_to_disk(void)
{
  int i;
  FILE *fp;
  struct poli_index_element *poli;

  sprintf(buf, "%s/%s.new", POL_PREFIX, POLI_FILE);
  if (!(fp = fopen(buf, "w+"))) {
    mudlog("SYSERR: OLC: Cannot open policy file!", BRF, LVL_IMMORT, TRUE);
    return;
  }
  for (i = 0; i <= top_of_polit; i++) {
    poli = (poli_table + i);

#if defined(PEDIT_LIST)
    sprintf(buf1, "OLC: Saving policy entry %d.", i);
    log(buf1);
#endif

    /*
     * Remove the '\r\n' sequences from description.
     */
    strcpy(buf1, poli->entry ? poli->entry : "Empty");
    strip_string(buf1);

    /*
     * Forget making a buffer, lets just write the thing now.
     */
    fprintf(fp, "%s\n%s%s#%d\n",
      poli->keywords ? poli->keywords : "UNDEFINED", buf1, (buf1[strlen(buf1) -1] == '\n') ? "" : "\n",
      poli->min_level);
  }

  /*
   * Write final line and close.
   */
  fprintf(fp, "$~\n");
  fclose(fp);
  sprintf(buf2, "%s/%s", POL_PREFIX, POLI_FILE);
  /*
   * We're fubar'd if we crash between the two lines below.
   */
  remove(buf2);
  rename(buf, buf2);

  olc_remove_from_save_list(PEDIT_PERMISSION, OLC_SAVE_POLI);
}

/*------------------------------------------------------------------------*/

void free_poli(struct poli_index_element *poli)
{

  if (poli->keywords)
    free(poli->keywords);
  if (poli->entry)
    free(poli->entry);
}

/**************************************************************************
 Menu functions 
 **************************************************************************/

/*
 * The main menu.
 */
void pedit_disp_menu(struct descriptor_data *d)
{
  struct poli_index_element *poli;

  get_char_cols(d->character);
  poli = OLC_POLI(d);

  sprintf(buf,
#if defined(CLEAR_SCREEN)
      "[H[J"
#endif
      "%s1%s) Keywords    : %s%s\r\n"
      "%s2%s) Entry       :\r\n%s%s"
      "%s3%s) Min Level   : %s%d\r\n"
      "%sQ%s) Quit\r\n"
      "Enter choice : ",

      grn, nrm, yel, poli->keywords,
      grn, nrm, yel, poli->entry,
      grn, nrm, cyn, poli->min_level,
      grn, nrm
      );
  send_to_char(buf, d->character);

  OLC_MODE(d) = PEDIT_MAIN_MENU;
}

/**************************************************************************
  The main loop
 **************************************************************************/

void pedit_parse(struct descriptor_data *d, char *arg)
{
  int number;

  switch (OLC_MODE(d)) {
  case PEDIT_CONFIRM_SAVESTRING:
    switch (*arg) {
    case 'y':
    case 'Y':
      pedit_save_internally(d);
      sprintf(buf, "OLC: %s edits policy for %s.", GET_NAME(d->character), OLC_POLI(d)->keywords);
      mudlog(buf, CMP, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), TRUE);
      /*
       * Do NOT free strings! Just the help structure. 
       */
      cleanup_olc(d, CLEANUP_STRUCTS);
      send_to_char("Policy entry saved to memory.\r\n", d->character);
      break;
    case 'n':
    case 'N':
      /*
       * Free everything up, including strings, etc.
       */
      cleanup_olc(d, CLEANUP_ALL);
      break;
    default:
      send_to_char("Invalid choice!\r\nDo you wish to save this policy entry internally? : ", d->character);
      break;
    }
    return;

  case PEDIT_MAIN_MENU:
    switch (*arg) {
    case 'q':
    case 'Q':
      if (OLC_VAL(d)) { /* Something has been modified. */
    send_to_char("Do you wish to save this policy entry internally? : ", d->character);
    OLC_MODE(d) = PEDIT_CONFIRM_SAVESTRING;
      } else
    cleanup_olc(d, CLEANUP_ALL);
      return;
    case '1':
      send_to_char("Enter keywords:-\r\n] ", d->character);
      OLC_MODE(d) = PEDIT_KEYWORDS;
      break;
    case '2':
      OLC_MODE(d) = PEDIT_ENTRY;
#if defined(CLEAR_SCREEN)
      SEND_TO_Q("\x1B[H\x1B[J", d);
#endif
      SEND_TO_Q("Enter policy entry: (/s saves /h for help)\r\n\r\n", d);
      d->backstr = NULL;
      if (OLC_POLI(d)->entry) {
    SEND_TO_Q(OLC_POLI(d)->entry, d);
    d->backstr = str_dup(OLC_POLI(d)->entry);
      }
      d->str = &OLC_POLI(d)->entry;
      d->max_str = MAX_POLI_ENTRY;
      d->mail_to[0] = 0;
      OLC_VAL(d) = 1;
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      break;
    case '3':
      send_to_char("Enter min level:-\r\n] ", d->character);
      OLC_MODE(d) = PEDIT_MIN_LEVEL;
      break;
    default:
      send_to_char("Invalid choice!\r\n", d->character);
      hedit_disp_menu(d);
      break;
    }
    return;

  case PEDIT_KEYWORDS:
    if (OLC_POLI(d)->keywords)
      free(OLC_POLI(d)->keywords);
    if (strlen(arg) > MAX_POLI_KEYWORDS)
      arg[MAX_POLI_KEYWORDS - 1] = '\0';
    OLC_POLI(d)->keywords = str_dup((arg && *arg) ? arg : "UNDEFINED");
    break;

  case PEDIT_ENTRY:
    /*
     * We will NEVER get here, we hope.
     */
    mudlog("SYSERR: Reached PEDIT_ENTRY case in parse_pedit", BRF, LVL_IMMORT, TRUE);
    break;

  case PEDIT_MIN_LEVEL:
    number = atoi(arg);
    if ((number < 0) || (number > LVL_IMPL))
      send_to_char("That is not a valid choice!\r\nEnter min level:-\r\n] ", d->character);
    else {
      OLC_POLI(d)->min_level = number;
      break;
    }
    return;

  default:
    /*
     * We should never get here.
     */
    mudlog("SYSERR: Reached default case in parse_pedit", BRF, LVL_IMMORT, TRUE);
    break;
  }
  /*
   * If we get this far, something has been changed.
   */
  OLC_VAL(d) = 1;
  pedit_disp_menu(d);
}

int find_poli_rnum(char *keyword)
{
  struct poli_indexer_element *el;
  
  if (!keyword || !*keyword) return -1;

  for (el = poli_index;el;el = el->next)
    if (!strncasecmp(keyword, el->keyword, strlen(keyword)))
      return el->pos;
    
  return -1;
}

int pedit_delete_entry(int rnum) {
  int i, j = 0;
  struct poli_index_element *new_poli_table;

  if ((rnum < 0) || (rnum > top_of_polit))
    return 0;

  CREATE(new_poli_table, struct poli_index_element, top_of_polit);

  /*
   * Count through help table.
   */
  for (i = 0; i <= top_of_polit; i++) {
    if (i != rnum) {
      new_poli_table[j] = poli_table[i];
      j++;
    } else {
      free_poli(poli_table + i);
    }
  }

  /*
   * Copy help table over to new one.
   */
  free(poli_table);
  poli_table = new_poli_table;
  top_of_polit--;
  olc_add_to_save_list(PEDIT_PERMISSION, OLC_SAVE_POLI);
  update_poli_index();
  return 1;
}

ACMD(do_peditutil) {
  int i;
  struct descriptor_data *d;
  
  if (IS_NPC(ch) || !ch->desc) {
    send_to_char("Go away!\r\n", ch);
    return;
  }

  for (d = descriptor_list; d; d = d->next)
    if (d->connected == CON_PEDIT) {
      sprintf(buf, "Policy files are already being editted by %s.\r\n",
              (CAN_SEE(ch, d->character) ? GET_NAME(d->character) : "someone"));
      send_to_char(buf, ch);
      return;
    }

  argument = one_word(argument, arg);
  skip_spaces(&argument);
  
  if (!arg || !*arg) {
    send_to_char("What do you wish to do to the policy entries?\r\n", ch);
    return;
  }
  
  if (!strcasecmp(arg, "save")) {
    send_to_char("Saving all policy information.\r\n", ch);
    sprintf(buf, "OLC: %s saves policy info", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
    mudlog(buf, CMP, LVL_IMMORT, TRUE);
    pedit_save_to_disk();
    return;
  }
  
  if (!strcasecmp(arg, "delete")) {
    i = find_poli_rnum(argument);
    if (i == -1) {
      send_to_char("Delete which policy entry?\r\n", ch);
      return;
    }
    sprintf(buf, "Deleting \'%s\' from the policy table.\r\n", poli_table[i].keywords);
    send_to_char(buf, ch);
    sprintf(buf, "OLC: %s deletes \'%s\' from the policy table", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), poli_table[i].keywords);
    mudlog(buf, CMP, LVL_IMMORT, TRUE);
    pedit_delete_entry(i);
    return;
  }
  send_to_char("What do you wish to do to the policy entries?\r\n", ch);
}
