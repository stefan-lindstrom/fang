#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "utils.h"
#include "db.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>

int xmlAtoi(char *string) {
  int tmp = -1;

  if (string) {
    tmp = atoi(string);
    free(string);
  }

  return (tmp);
}

long xmlAtol(char *string) {
  long tmp = -1;

  if (string) {
    tmp = atol(string);
    free(string);
  }

  return (tmp);
}

xmlNodePtr xmlNullTest(xmlNodePtr *node) {
  if (xmlIsBlankNode(*node) && !(*node)->children && !(*node)->properties) {
    xmlFreeNode(*node);
    *node = NULL;
  }
  return *node;
}

void addStringToXml(xmlNodePtr node, const char *tag, const char *entry) {
  xmlNodePtr xmlTmp;
  char *tmp;

  if (!node || !tag || !*tag || !entry || !*entry)
    return;

  xmlTmp = xmlNewNode(NULL, tag);
  xmlTmp->doc = node->doc;

  tmp = xmlEncodeEntitiesReentrant(node->doc, entry);
  xmlNodeSetContent(xmlTmp, tmp);
  free(tmp);

  xmlAddChild(node, xmlTmp);
}

void addIntToXml(xmlNodePtr node, const char *tag, int num) {
  xmlNodePtr xmlTmp;

  if (!node || !tag || !*tag)
    return;

  xmlTmp = xmlNewNode(NULL, tag);
  xmlTmp->doc = node->doc;

  sprintf(buf, "%d", num);
  xmlNodeSetContent(xmlTmp, buf);

  xmlAddChild(node, xmlTmp);
}

void addLongToXml(xmlNodePtr node, const char *tag, long num) {
  xmlNodePtr xmlTmp;

  if (!node || !tag || !*tag)
    return;

  xmlTmp = xmlNewNode(NULL, tag);
  xmlTmp->doc = node->doc;

  sprintf(buf, "%ld", num);
  xmlNodeSetContent(xmlTmp, buf);

  xmlAddChild(node, xmlTmp);
}

void addCharToXml(xmlNodePtr node, const char *tag, char num) {
  xmlNodePtr xmlTmp;

  if (!node || !tag || !*tag)
    return;

  xmlTmp = xmlNewNode(NULL, tag);
  xmlTmp->doc = node->doc;

  sprintf(buf, "%c", num); 
  xmlNodeSetContent(xmlTmp, buf);
 
  xmlAddChild(node, xmlTmp);
}

