#include "SVGParser.h"
#include <ctype.h>

/*
 *  CIS*27500 Assignment #2
 *  author: Cameron Norrie (1046856)
 *  Project: SVGHelpers.h
 *  Date: March 2, 2022
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

//A2
void svgToDoc(const SVG *img, xmlNode **root_node, xmlDoc **doc);
void rectangleDoc(xmlNode *root_node, List *rectangleList);
void otherAttrDoc(xmlNode *root_node, List *otherAttrList);
void circleDoc(xmlNode *root_node, List *circleList);
void pathDoc(xmlNode *root_node, List *pathList);
void groupDoc(xmlNode *root_node, List *groupList);

bool validateAgainstXSD(const char* schemaFile, xmlDoc *doc);

bool checkImage(const SVG *img);
bool validateRects(List* rectangleList);
bool validateAttributes(List* otherAttrList);
bool validateCircles(List* circleList);
bool validatePaths(List* pathList);
bool validateGroups(List* groupList);
bool validateSvgContraints(const SVG *img);

bool setSvgAttr(SVG* img, Attribute* newAttribute);
bool setCircleAttr(SVG* img, int elemIndex, Attribute* newAttribute);
bool setOtherAttr(List *otherAttr, Attribute* newAttribute);
bool setRectangleAttr(SVG* img, int elemIndex, Attribute* newAttribute);
bool setPathAttr(SVG* img, int elemIndex, Attribute* newAttribute);
bool setGroupAttr(SVG* img, int elemIndex, Attribute* newAttribute);

// A3
char* createSVGlogs(char* filename);
char* fillSVGViewPanel(char* filename);
char* imgTitle(char *filename);
char* imgDescription(char *filename);
void editTitleDesc(char* filename, char* title, char* description);
int createNewSVGfromInput(char* filename, char* title, char* description);