#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVGParser.h"
#include "SVGHelpers.h"
#include <math.h>

/*
 *  CIS*27500 Assignment #1
 *  author: Cameron Norrie (1046856)
 *  Project: SVGParser.c
 *  Date: February 4, 2022
 */

SVG* createSVG(const char* fileName) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    LIBXML_TEST_VERSION

    // parse the file and get the document tree
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        printf("Error: could not parse the provided file %s.", fileName);
    
        return NULL;
    }

    // Get the root element node
    root_element = xmlDocGetRootElement(doc);
    
    //Creating an empty SVGimage struct
    SVG *svg = createSVGImage();

    parseSVGImage(root_element, svg);

    // free the document
    xmlFreeDoc(doc);

    // Free the global variables that may have been allocated by the parser.
    xmlCleanupParser();

    return svg;
}

char* SVGToString(const SVG* img) {

    if (img == NULL) {
        return NULL;
    }

    char* string = malloc(15);

    char *stringPath = toString(img->paths);
    char *stringCircle = toString(img->circles);
    char *stringRectangle = toString(img->rectangles);
    char *stringGroups = toString(img->groups);
    char *otherAttr = toString(img->otherAttributes);

    char *tempString = malloc(sizeof(char) * 50000);
    sprintf(string, "-- SVG --\n");

    // namespace
    if (strlen(img->title) > 0) {
        sprintf(tempString, "Namespace: %s\n", img->namespace);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // title
    if (strlen(img->title) > 0) {
        sprintf(tempString, "Title: %s\n", img->title);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // description
    if (strlen(img->description) > 0) {
        sprintf(tempString, "Description: \n%s\n", img->description);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // rectangles
    if (img->rectangles->length > 0) {
        sprintf(tempString, "Rectangles: \n%s\n", stringRectangle);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // circles
    if (img->circles->length > 0) {
        sprintf(tempString, "Circles: \n%s\n", stringCircle);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // paths
    if (img->paths->length > 0) {
        sprintf(tempString, "Paths: \n%s\n", stringPath);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }
    // groups
    if (img->groups->length > 0) {
        sprintf(tempString, "Group: \n%s\n", stringGroups);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // Other Attributes
    if(strlen(otherAttr) > 0){
        sprintf(tempString, "Other Attributes: %s\n", otherAttr);
        string = realloc(string, strlen(tempString) + strlen(string) + 2);
        strcat(string, tempString);
    }

    //end statement
    sprintf(tempString, "-- End SVG --");
    string = realloc(string, strlen(tempString) + strlen(string) + 10);
    strcat(string, tempString);

    // free toStrings
    free(tempString);
    free(stringPath);
    free(stringCircle);
    free(stringRectangle);
    free(stringGroups);
    free(otherAttr);

    return string;
}

//deletes an SVG struct
void deleteSVG(SVG* img) {
    if (img == NULL) {
        return;
    }

    freeList(img->otherAttributes);
    freeList(img->groups);
    freeList(img->circles);
    freeList(img->rectangles);
    freeList(img->paths);

    free(img);
    img = NULL;

    /* Frees global variables that might have been allocated by the XMLlib */
    xmlCleanupParser();
}

//deletes a rectangle struct
void deleteRectangle(void* data) {
    if (data == NULL){
        return;
    }

    Rectangle *rect = (Rectangle *)data;
    
    freeList(rect->otherAttributes);
    free(rect);
}

//turns a rectangle struct into a string
char* rectangleToString(void* data) {
    if (data == NULL) {
        return NULL;
    }

    Rectangle *rect = (Rectangle *)data;
    char* string = malloc(1);

    char *otherAttr = toString(rect->otherAttributes);

    char tempString[500];

    // x
    sprintf(tempString, "x: %f\n", rect->x);
    string = realloc(string, strlen(tempString) + 2);
    strcpy(string, tempString);

    // y
    sprintf(tempString, "y: %f\n", rect->y);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // width
    sprintf(tempString, "width: %f\n", rect->width);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // height
    sprintf(tempString, "height: %f\n", rect->height);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // units
    sprintf(tempString, "units: %s\n", rect->units);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // Other Attributes
    if(strlen(otherAttr) > 0){
        sprintf(tempString, "Other Attributes: %s\n", otherAttr);
        string = realloc(string, strlen(tempString) + strlen(string) + 2);
        strcat(string, tempString);
    }
    
    free(otherAttr);
    return string;
}

int compareRectangles(const void *first, const void *second) {

    return 0;
}

//deletes an attribute struct
void deleteAttribute( void* data) {
    if (data == NULL){
        return;
    }

    Attribute *attribute = (Attribute*)data;
    free(attribute->name);
    free(attribute);
}

//turns an attribute struct into a string
char* attributeToString( void* data){
    if (data == NULL){
        return NULL;
    } 

    Attribute *attr = (Attribute *)data;
    char *string = (char *)calloc(10000, sizeof(char));

    sprintf(string, "\nAttribute name: %s\tAttribute value: %s\n", attr->name, attr->value);

    return string;
}

int compareAttributes(const void *first, const void *second) {

    return 0;
}

//deletes a circle struct
void deleteCircle(void* data){
    if (data == NULL){
        return;
    }

    Circle *circ = (Circle *)data;
    freeList(circ->otherAttributes);
    free(circ);
}

//turns a circle struct into a string
char* circleToString(void* data) {
    if (data == NULL){
        return NULL;
    }

    Circle *circle = (Circle *)data;

    char* string = malloc(1);

    char *otherAttr = toString(circle->otherAttributes);

    char tempString[500];

    // cx
    sprintf(tempString, "cx: %f\n", circle->cx);
    string = realloc(string, strlen(tempString) + 2);
    strcpy(string, tempString);

    // cy
    sprintf(tempString, "cy: %f\n", circle->cy);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // r
    sprintf(tempString, "r: %f\n", circle->r);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // units
    sprintf(tempString, "units: %s\n", circle->units);
    string = realloc(string, strlen(tempString) + strlen(string) + 2);
    strcat(string, tempString);

    // Other Attributes
    if(strlen(otherAttr) > 0){
        sprintf(tempString, "Other Attributes: %s\n", otherAttr);
        string = realloc(string, strlen(tempString) + strlen(string) + 2);
        strcat(string, tempString);
    }

    free(otherAttr);
    return string;
}

int compareCircles(const void *first, const void *second) {

    return 0;
}

//deletes a path struct
void deletePath(void* data) {
    if (data == NULL){
        return;
    }

    Path *path = (Path*)data;

    freeList(path->otherAttributes);
    free(path);
}

//turns a path struct into a string
char* pathToString(void* data) {
    if (data == NULL){
        return NULL;
    }

    Path *path = (Path *)data;

    char* string = malloc(1);
    char *otherAttr = toString(path->otherAttributes);
    char tempString[500];

    // data
    string = realloc(string, strlen(path->data) + 15);
    sprintf(string, "Data: %s\n", path->data);

    // Other Attributes
    if(strlen(otherAttr) > 0){
        sprintf(tempString, "\nOther attributes: %s\n", otherAttr);
        string = realloc(string, strlen(tempString) + strlen(string) + 5);
        strcat(string, tempString);
    }
    
    free(otherAttr);

    return string;
}

int comparePaths(const void *first, const void *second) {
   
    return 0;
}

void deleteGroup(void* data) {
    if (data == NULL){
        return;
    }

    Group *group = (Group *)data;
    freeList(group->otherAttributes);
    freeList(group->groups);
    freeList(group->circles);
    freeList(group->rectangles);
    freeList(group->paths);
    free(group);
}

char* groupToString( void* data) {
    if (data == NULL){
        return NULL;
    }

    Group *group = (Group *)data;

    char* string = malloc(15);

    char *stringPath = toString(group->paths);
    char *stringCircle = toString(group->circles);
    char *stringRectangle = toString(group->rectangles);
    char *stringGroups = toString(group->groups);

    char tempString[500];
    sprintf(string, "-- Group --\n");

    // rectangles
    if (group->rectangles->length > 0) {
        sprintf(tempString, "Rectangles: \n%s\n", stringRectangle);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // circles
    if (group->circles->length > 0) {
        sprintf(tempString, "Circles: \n%s\n", stringCircle);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // paths
    if (group->paths->length > 0) {
        sprintf(tempString, "Paths: \n%s\n", stringPath);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }
    // groups
    if (group->groups->length > 0) {
        sprintf(tempString, "Internal Group: \n%s\n", stringGroups);
        string = realloc(string, strlen(tempString) + strlen(string) + 10);
        strcat(string, tempString);
    }

    // end statement
    sprintf(tempString, "-- End Group --\n");
    string = realloc(string, strlen(tempString) + strlen(string) + 10);
    strcat(string, tempString);

    // free toStrings
    free(stringPath);
    free(stringCircle);
    free(stringRectangle);
    free(stringGroups);

    return string;
}

int compareGroups(const void *first, const void *second) {

    return 0;
}

// Function that returns a list of all rectangles in the struct.  
List* getRects(const SVG* img) {
    // checking if svg exists
    if (img == NULL) {
        return NULL;
    }

    List *rectangleList = initializeList(rectangleToString, deleteRectangle, compareRectangles);

    // checking if rectangles exist
    if(img->rectangles == NULL && getLength(rectangleList) <= 0) {
        return NULL;
    }

    // rectangles
    ListIterator rectIter = createIterator(img->rectangles);
    Rectangle *rect = nextElement(&rectIter);

    while(rect != NULL){
        insertBack(rectangleList, rect);
        rect = nextElement(&rectIter);
    }
    
    // checking in groups                                                                 
    ListIterator groupIter = createIterator(img->groups);
    Group *group = nextElement(&groupIter);

    while(group != NULL) {
        rectGroup(group,rectangleList);
        group = nextElement(&groupIter);
    }

    return rectangleList;
}

// Function that returns a list of all circles in the struct.  
List* getCircles(const SVG* img) {
    // checking if svg exists
    if (img == NULL) {
        return NULL;
    }

    List *circleList = initializeList(circleToString, deleteCircle, compareCircles);

    // checking if rectangles exist
    if(img->circles == NULL && getLength(circleList) <= 0) {
        return NULL;
    }

    // circles
    ListIterator circIter = createIterator(img->circles);
    Circle *circ = nextElement(&circIter);

    while(circ != NULL){
        insertBack(circleList, circ);
        circ = nextElement(&circIter);
    }

    // checking in groups                                                                 
    ListIterator groupIter = createIterator(img->groups);
    Group *group = nextElement(&groupIter);

    while(group != NULL) {
        circGroup(group,circleList);
        group = nextElement(&groupIter);
    }

    return circleList;
}

// Function that returns a list of all groups in the struct.  
List* getGroups(const SVG* img){
    // checking if svg exists
    if (img == NULL) {
        return NULL;
    }

    List *groupList = initializeList(groupToString, deleteGroup, compareGroups);

    // checking if groups exist
    if(img->groups == NULL && getLength(groupList) <= 0) {
        return NULL;
    }

    ListIterator groupIter = createIterator(img->groups);
    ListIterator *groupVar = &groupIter;
    Group *group = nextElement(groupVar);

    while(group != NULL) {
        groupChecker(group,groupList);
        insertBack(groupList, group);
        group = nextElement(groupVar);
    }

    return groupList;
}

// Function that returns a list of all paths in the struct.  
List* getPaths(const SVG* img){
    // checking if svg exists
    if (img == NULL) {
        return NULL;
    }

    List *pathList = initializeList(pathToString, deletePath, comparePaths);

    // checking if paths exist
    if(img->paths == NULL && getLength(pathList) <= 0) {
        return NULL;
    }

    // paths
    ListIterator pathIter = createIterator(img->paths);
    Path *path = nextElement(&pathIter);

    while(path != NULL){
        insertBack(pathList, path);
        path = nextElement(&pathIter);
    }

    // checking in groups                                                                 
    ListIterator groupIter = createIterator(img->groups);
    Group *group = nextElement(&groupIter);

    while(group != NULL) {
        pathGroup(group, pathList);
        group = nextElement(&groupIter);
    }

    return pathList;
}

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(const SVG* img, float area){
    // checking if svg exists
    if (img == NULL) {
        return 0;
    }
    List *rectList = getRects(img);
    ListIterator iterator = createIterator(rectList);
    void *rectElement;
    float currArea = 0;
    float givenArea = 0;
    int numOfRects = 0;
    float x = 0; 
    float y = 0;
    Rectangle *rect;

    // round up the given area
    givenArea = ceil(area);

    while ((rectElement = nextElement(&iterator))) {
        rect = (Rectangle *)rectElement;

        // getting variables
        x = rect->height;
        y = rect->width;

        // calculate area rounding up
        currArea = ceil(x * y);

        // checking for match
        if (currArea == givenArea) {
            numOfRects++;
        }
    }

    deleteList(rectList);
    free(rectList);

    return numOfRects;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(const SVG* img, float area){
    // checking if svg exists
    if (img == NULL) {
        return 0;
    }

    List *circList = getCircles(img);
    ListIterator iterator = createIterator(circList);
    void *circElement;
    float currArea = 0;
    float givenArea = 0;
    int numOfCircles = 0;
    float r = 0; 
    Circle *circ;

    // round up the given area
    givenArea = ceil(area);

    while ((circElement = nextElement(&iterator))) {
        circ = (Circle *)circElement;

        // getting variables
        r = circ->r;

        // calculate area rounding up
        currArea = ceil(r * r * 3.14159265358979323846);

        // checking for match
        if (currArea == givenArea) {
            numOfCircles++;
        }
    }

    deleteList(circList);
    free(circList);

    return numOfCircles;
}

// Function that returns the number of all paths with the specified data
int numPathsWithdata(const SVG* img, const char* data) {
    // checking if svg exists
    if (img == NULL) {
        return 0;
    }

    List *pathList = getPaths(img);
    ListIterator iterator = createIterator(pathList);
    void *pathElement;
    int numOfPaths = 0;
    Path *path;

    while ((pathElement = nextElement(&iterator))) {
        path = (Path *)pathElement;

        if (strcmp(data, path->data) == 0) {
            numOfPaths++;
        }
    }

    deleteList(pathList);
    free(pathList);

    return numOfPaths;
}

// Function that returns the number of all groups with the specified length
int numGroupsWithLen(const SVG* img, int len) {
    // checking if svg exists
    if (img == NULL) {
        return 0;
    }

    List *groupList = getGroups(img);
    ListIterator iterator = createIterator(groupList);
    void *groupElement;
    int numOfGroups = 0;
    int groupLength;
    Group *groups;

    while ((groupElement = nextElement(&iterator))) {
        groups = (Group *)groupElement;

        groupLength = 0;

        // groupLength =  groupLength + groups->rectangles->length;
        // groupLength =  groupLength + groups->circles->length;
        // groupLength =  groupLength + groups->paths->length;
        // groupLength =  groupLength + groups->groups->length;

        groupLength = (groups->rectangles->length + groups->circles->length 
        + groups->paths->length + groups->groups->length);

        if (len == groupLength) {
            numOfGroups++;
        }
    }

    deleteList(groupList);
    free(groupList);

    return numOfGroups;
}

// Function that returns the number of all attributes within the svg file
int numAttr(const SVG* img) {
    // checking if svg exists
    if (img == NULL) {
        return 0;
    }

    int numOfAttr = 0;
    void *attrElement = NULL;

    //Start count here
    numOfAttr = img->otherAttributes->length; 

    // rectangles
    List *list = getRects(img);
    ListIterator iterator = createIterator(list);

    while ((attrElement = nextElement(&iterator)) != NULL) {
        Rectangle *rect = (Rectangle *)attrElement;

        numOfAttr += rect->otherAttributes->length;
    }
    deleteList(list);
    free(list);

    // circles
    list = getCircles(img);
    iterator = createIterator(list);

    while ((attrElement = nextElement(&iterator))) {
        Circle *circ = (Circle *)attrElement;
        numOfAttr += circ->otherAttributes->length;
    }
    deleteList(list);
    free(list);

    // paths
    list = getPaths(img);
    iterator = createIterator(list);

    while ((attrElement = nextElement(&iterator))) {
        Path *path = (Path *)attrElement;
        numOfAttr += path->otherAttributes->length;
    }
    deleteList(list);
    free(list);

    // groups
    list = getGroups(img);
    iterator = createIterator(list);

    while ((attrElement = nextElement(&iterator))) {
        Group *groups = (Group *)attrElement;
        numOfAttr += groups->otherAttributes->length;
    }
    deleteList(list);
    free(list);

    return numOfAttr;
}
