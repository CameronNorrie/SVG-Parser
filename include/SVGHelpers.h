#include "SVGParser.h"
#include <ctype.h>

/*
 *  CIS*27500 Assignment #1
 *  author: Cameron Norrie (1046856)
 *  Project: SVGHelpers.h
 *  Date: February 4, 2022
 */

void parseSVGImage(xmlNode * a_node, SVG *img);

SVG* createSVGImage();

Rectangle *createRectangle();
Attribute *createAttribute();
Circle *createCircle();
Path *createPath();
Group *createGroup();

void addRectangle(xmlNode *cur_node, List *listToAdd);
void addCircle(xmlNode *cur_node, List *listToAdd);
void addPath(xmlNode *cur_node, List *listToAdd);
void addGroup(xmlNode *cur_node, List *listToAdd);
void addUnits(char *unitChar, char *searchContent);

void deleteList(List* list);

void rectGroup(Group *group, List *rectangleList);
void circGroup(Group *group, List *circleList);
void groupChecker(Group *group, List *list);
void pathGroup(Group *group, List *pathList);