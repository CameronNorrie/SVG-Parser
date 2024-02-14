#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVGParser.h"
#include "SVGHelpers.h"
#include <math.h>

/*
 *  CIS*27500 Assignment #2
 *  author: Cameron Norrie (1046856)
 *  Project: SVGHelpers.c
 *  Date: March 2, 2022
 */

/*my helper functions*/

void parseSVGImage(xmlNode * a_node, SVG *img) {
    xmlNode *cur_node = NULL;

    //looping through the svg file
    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        //grabbing namespace
        if (cur_node->ns != NULL) {
            xmlNs *namespace = cur_node->ns;
            //checking to see if namespace is null
            if ((char *)namespace->href == NULL){
                strcpy(img->namespace, "");
            //adding the namespace
            } else {
                strcpy(img->namespace, (char *)namespace->href);
            }
        }

        //title
        if (strcmp((char*)cur_node->name, "title") == 0) {
            char temp[255] = "";
            strncpy(temp, (char *)cur_node->children->content, 255);
            strcpy(img->title, temp);

        //description
        } else if (strcmp((char*)cur_node->name, "desc") == 0) {
            char temp[255] = "";
            strncpy(temp, (char *)cur_node->children->content, 255);
            strcpy(img->description, temp);

        //rectangles struct is filled
        } else if (strcmp((char*)cur_node->name, "rect") == 0) {
           addRectangle(cur_node, img->rectangles);

        //circles struct is filled
        } else if (strcmp((char*)cur_node->name, "circle") == 0) { 
            addCircle(cur_node, img->circles);
            
        // COULD USE SOME WORK POTENTIALLY WITH MEMORY
        } else if (strcmp((char*)cur_node->name, "path") == 0) {
            addPath(cur_node, img->paths);
            
        } else if (strcmp((char *)cur_node->name, "g") == 0) {
            addGroup(cur_node, img->groups);

        }

        // grabbing otherAttributes from svg
        for (xmlAttr* attr = cur_node->properties; attr != NULL  && strcmp("svg", (char*)attr->parent->name) == 0; attr = attr->next) {
            Attribute *att = createAttribute();
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);

            strcpy(att->name, attrName);
            strcpy(att->value, cont);

            insertBack(img->otherAttributes, (void *)att);
        }
        
        if (cur_node->next == NULL && cur_node->children != NULL) { 
            // Goes 1 level deeper
            cur_node = cur_node->children;
        }
    }
}

void addUnits(char *unitChar, char *searchContent) {

    // checks if unit has already been found
    if (strlen(unitChar) > 0) {
        return;
    }

    int index = 0;
    
    //find where the units start
    while (index < strlen(searchContent) && !isalpha(searchContent[index])){
        index++;
    }
    
    // place the units into the unit array and null terminate end of the unit
    if (index != strlen(searchContent)) {
        strncpy(unitChar, searchContent + index, strlen(searchContent) - index);
        unitChar[strlen(searchContent)] = '\0';
    }
}

SVG* createSVGImage(){
    SVG *svg = malloc(sizeof(SVG));

    strcpy(svg->namespace, "");
    strcpy(svg->title, "");
    strcpy(svg->description, "");

    //initialize the lists
    svg->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    svg->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    svg->circles = initializeList(circleToString, deleteCircle, compareCircles);
    svg->paths = initializeList(pathToString, deletePath, comparePaths);
    svg->groups = initializeList(groupToString, deleteGroup, compareGroups);

    return svg;
}

//creates a rectangle struct
Rectangle *createRectangle() {
    Rectangle *rectangles = malloc(sizeof(Rectangle)); 
    
    rectangles->x = 0;
    rectangles->y = 0;
    rectangles->width = 0;
    rectangles->height = 0;
    rectangles->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(rectangles->units, "");

    return rectangles;
}

void addRectangle(xmlNode *cur_node, List *listToAdd) {
    xmlAttr *attr;

    Rectangle *rectangles = createRectangle();
    //looping through node for all rectangle content
    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        if (strcmp(attrName, "x") == 0) {             	
            float temp;

            temp = atof((char*)cont);
            rectangles->x = temp;
            addUnits(rectangles->units, cont);

        } else if (strcmp(attrName, "y") == 0) {       
            float temp;

            temp = atof((char*)cont);
            rectangles->y = temp;
            addUnits(rectangles->units, cont);

        } else if (strcmp(attrName, "width") == 0) {     
            float temp;

            temp = atof((char*)cont);
            rectangles->width = temp;
            addUnits(rectangles->units, cont);

        } else if (strcmp(attrName, "height") == 0) {       
            float temp;

            temp = atof((char*)cont);
            rectangles->height = temp;
            addUnits(rectangles->units, cont);

            //where other attributes will be stored
        } else {
            Attribute *att = createAttribute();

            strcpy(att->name, attrName);
            strcpy(att->value, cont);

            insertBack(rectangles->otherAttributes, (void *)att);
        }
    }
    //insert rectangle into the back of the list
    insertBack(listToAdd, (void *)rectangles);
}

Attribute *createAttribute(){
    Attribute *attribute = malloc(sizeof(Attribute) + 250 * sizeof(char));
    attribute->name = malloc(sizeof(char) * 250);

    strcpy(attribute->name, "");
    strcpy(attribute->value, "");

    return attribute;
}

//creates a circle struct
Circle *createCircle(){
    Circle *circle = malloc(sizeof(Circle));

    circle->cx = 0;
    circle->cy = 0;
    circle->r = 0;
    circle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(circle->units, "");

    return circle;
}

void addCircle(xmlNode *cur_node, List *listToAdd) {
    xmlAttr *attr;
    Circle *circles = createCircle();
    //looping through node for all circle content
    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        if (strcmp(attrName, "cx") == 0) {             	
            float temp;

            temp = atof((char*)cont);
            circles->cx = temp;
            addUnits(circles->units, cont);

        } else if (strcmp(attrName, "cy") == 0) {       
            float temp;

            temp = atof((char*)cont);
            circles->cy = temp;
            addUnits(circles->units, cont);

        } else if (strcmp(attrName, "r") == 0) {     
            float temp;

            temp = atof((char*)cont);
            circles->r = temp;
            addUnits(circles->units, cont);

        //where other attributes will be stored
        } else {
            Attribute *att = createAttribute();

            strcpy(att->name, attrName);
            strcpy(att->value, cont);
                    
            insertBack(circles->otherAttributes, (void *)att);
        }
    }
    //insert circle into the back of the list
    insertBack(listToAdd, (void *)circles);
}

Path *createPath(xmlNode *cur_node){
    
    int length = strlen((char *)(cur_node->properties->children->content)) + 1;

    Path *path = malloc(sizeof(Path) + length * sizeof(char) + 500);
    path->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(path->data, ""); 

    return path;
}

void addPath(xmlNode *cur_node, List *listToAdd) {
    xmlAttr *attr;
    Path *paths = createPath(cur_node);

    //looping through node for all path content
    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;
        char *cont = "";
        char *attrName = "";

        attrName = (char *)attr->name;
        cont = (char *)(value->content);

        if (strcmp(attrName, "d") == 0) {
            int length = strlen((char *)(cont)) + 1;
            paths = realloc(paths, length * sizeof(char) + 500);
            strcpy(paths->data, cont);

        } else {
            Attribute *att = createAttribute();

            strcpy(att->name, attrName);
            strcpy(att->value, cont);

            insertBack(paths->otherAttributes, (void *)att);
        }
    }
    //insert path into the back of the list
    insertBack(listToAdd, (void *)paths);
}

void addGroup(xmlNode *anode, List *listToAdd){
    xmlAttr *attr;

    Group *groups = createGroup();
    //looping through node for all other attribute content
    for (attr = anode->properties; attr != NULL; attr = attr->next) {
        xmlNode* value = attr->children;
        char *cont = "";
        char *attrName = "";

        attrName = (char *)attr->name;
        cont = (char *)(value->content);
        Attribute *att = createAttribute();

        strcpy(att->name, attrName);
        strcpy(att->value, cont);

        insertBack(groups->otherAttributes, (void *)att);      
    }
    for (xmlNode* cur_node = anode->children; cur_node; cur_node = cur_node->next) {

        //rectangle struct is filled
        if (strcmp((char*)cur_node->name, "rect") == 0) {
           addRectangle(cur_node, groups->rectangles);

        //circles struct is filled
        } else if (strcmp((char*)cur_node->name, "circle") == 0) { 
            addCircle(cur_node, groups->circles);
            
        //paths struct is filled
        } else if (strcmp((char*)cur_node->name, "path") == 0) {
            addPath(cur_node, groups->paths);
        
        //groups struct is filled
        } else if (strcmp((char *)cur_node->name, "g") == 0) {
            addGroup(cur_node, groups->groups);

        }

        if (cur_node->next == NULL && cur_node->children != NULL) { // Only go one level deeper if there's another node (tag) to traverse
            // Goes 1 level deeper
            cur_node = cur_node->children;
        }
    }
    insertBack(listToAdd, (void *)groups);
}

Group *createGroup(){
    Group *group = malloc(sizeof(Group));
    group->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    group->paths = initializeList(pathToString, deletePath, comparePaths);
    group->circles = initializeList(circleToString, deleteCircle, compareCircles);
    group->groups = initializeList(groupToString, deleteGroup, compareGroups);
    group->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    return group;
}

// This function uses clearList provided by LinkedListAPI.c without the dealeting of data
void deleteList(List* list){	
    if (list == NULL){
		return;
	}
	
	if (list->head == NULL && list->tail == NULL){
		return;
	}
	
	Node* tmp;
	
	while (list->head != NULL){
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}
	
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
}

// helper function to find rectangles in groups
void rectGroup(Group *group, List *rectangleList) {
    // checking if group exists
    if(group == NULL) {
        return;
    }

    // rectangles in groups
    ListIterator rectIter = createIterator(group->rectangles);
    Rectangle *rect = nextElement(&rectIter);

    while(rect != NULL){
        insertBack(rectangleList, rect);
        rect = nextElement(&rectIter);
    }

    // checking groups
    ListIterator groupIter = createIterator(group->groups);
    Group *newGroup = nextElement(&groupIter);

    while(newGroup != NULL) {
        rectGroup(newGroup, rectangleList);
        newGroup = nextElement(&groupIter);
    }
}

// helper function to find circles in groups
void circGroup(Group *group, List *circleList) {
    // checking if group exists
    if(group == NULL) {
        return;
    }

    // circles in groups
    ListIterator circIter = createIterator(group->circles);
    Circle *circ = nextElement(&circIter);

    while(circ != NULL){
        insertBack(circleList, circ);
        circ = nextElement(&circIter);
    }

    // checking groups
    ListIterator groupIter = createIterator(group->groups);
    Group *newGroup = nextElement(&groupIter);

    while(newGroup != NULL) {
        circGroup(newGroup, circleList);
        newGroup = nextElement(&groupIter);
    }
}

// helper function to find rectangles in groups
void groupChecker(Group *group, List *list) {
    // checking if group exists
    if(group == NULL) {
        return;
    }

    // checking groups
    ListIterator groupIter = createIterator(group->groups);
    Group *newGroup = nextElement(&groupIter);

    while(newGroup != NULL) {
        groupChecker(newGroup, list);
        insertBack(list, newGroup);
        newGroup = nextElement(&groupIter);
    }
}

// helper function to find rectangles in groups
void pathGroup(Group *group, List *pathList) {
    // checking if group exists
    if(group == NULL) {
        return;
    }

    // paths in groups
    ListIterator pathIter = createIterator(group->paths);
    Path *path = nextElement(&pathIter);

    while(path != NULL){
        insertBack(pathList, path);
        path = nextElement(&pathIter);
    }

    // checking groups
    ListIterator groupIter = createIterator(group->groups);
    Group *newGroup = nextElement(&groupIter);

    while(newGroup != NULL) {
        pathGroup(newGroup, pathList);
        newGroup = nextElement(&groupIter);
    }
}

void svgToDoc(const SVG *img, xmlNode **root_node, xmlDoc **doc) {
    //creating new doc
    *doc = xmlNewDoc((xmlChar*)"1.0");
    *root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlDocSetRootElement(*doc, *root_node);

    // adding namespace to the doc
    xmlNsPtr namespace = xmlNewNs(*root_node, BAD_CAST img->namespace, BAD_CAST NULL);
    xmlSetNs(*root_node, namespace);

    //title
    if (strlen(img->title) > 0) {
        xmlNewChild(*root_node, NULL, BAD_CAST "title", BAD_CAST img->title);
    }

    //description
    if (strlen(img->description) > 0) {
        xmlNewChild(*root_node, NULL, BAD_CAST "desc", BAD_CAST img->description);
    }

    //other attributes
    otherAttrDoc(*root_node, img->otherAttributes);

    //rectangles 
    rectangleDoc(*root_node, img->rectangles);

    //circles 
    circleDoc(*root_node, img->circles);
            
    // paths
    pathDoc(*root_node, img->paths);

    //groups    
    groupDoc(*root_node, img->groups);
}

void rectangleDoc(xmlNode *root_node, List *rectangleList) {
    List *rectList = rectangleList;
    ListIterator iterator = createIterator(rectList);
    void *rectElement;
    xmlNode *rectNode;
    Rectangle *rect;
    char *temp = malloc(sizeof(char) + 10000);

    while ((rectElement = nextElement(&iterator))) {
        rect = (Rectangle *)rectElement;
        rectNode = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);

        if (strlen(rect->units) != 0) {
            sprintf(temp, "%f", rect->x);
            strcat(temp, rect->units);
            xmlNewProp(rectNode, BAD_CAST "x", BAD_CAST temp);

            sprintf(temp, "%f", rect->y);
            strcat(temp, rect->units);
            xmlNewProp(rectNode, BAD_CAST "y", BAD_CAST temp);

            sprintf(temp, "%f", rect->width);
            strcat(temp, rect->units);
            xmlNewProp(rectNode, BAD_CAST "width", BAD_CAST temp);

            sprintf(temp, "%f", rect->height);
            strcat(temp, rect->units);
            xmlNewProp(rectNode, BAD_CAST "height", BAD_CAST temp);
        } else {
            sprintf(temp, "%f", rect->x);
            xmlNewProp(rectNode, BAD_CAST "x", BAD_CAST temp);

            sprintf(temp, "%f", rect->y);
            xmlNewProp(rectNode, BAD_CAST "y", BAD_CAST temp);

            sprintf(temp, "%f", rect->width);
            xmlNewProp(rectNode, BAD_CAST "width", BAD_CAST temp);

            sprintf(temp, "%f", rect->height);
            xmlNewProp(rectNode, BAD_CAST "height", BAD_CAST temp);
        }

        if (rect->otherAttributes != NULL) {
            if (rect->otherAttributes->length) {
                otherAttrDoc(rectNode, rect->otherAttributes);
            }
        }
    }
    free(temp);
}

void otherAttrDoc(xmlNode *root_node, List *otherAttrList){
    List *attrList = otherAttrList;
    ListIterator iterator = createIterator(attrList);
    void *attrElement;
    Attribute *attr;

    while ((attrElement = nextElement(&iterator))) {
        attr = (Attribute *)attrElement;
        xmlNewProp(root_node, BAD_CAST attr->name, BAD_CAST attr->value);
    }
}

void circleDoc(xmlNode *root_node, List *circleList) {
    List *circList = circleList;
    ListIterator iterator = createIterator(circList);
    void *circElement;
    xmlNode *circNode;
    Circle *circ;
    char *temp = malloc(sizeof(char) + 10000);

    while ((circElement = nextElement(&iterator))) {
        circ = (Circle *)circElement;
        circNode = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);

        sprintf(temp, "%f", circ->cx);
        strcat(temp, circ->units);
        xmlNewProp(circNode, BAD_CAST "cx", BAD_CAST temp);

        sprintf(temp, "%f", circ->cy);
        strcat(temp, circ->units);
        xmlNewProp(circNode, BAD_CAST "cy", BAD_CAST temp);

        sprintf(temp, "%f", circ->r);
        strcat(temp, circ->units);
        xmlNewProp(circNode, BAD_CAST "r", BAD_CAST temp);

        if (circ->otherAttributes != NULL) {
            if (circ->otherAttributes->length) {
                otherAttrDoc(circNode, circ->otherAttributes);
            }
        }

    }
    free(temp);
}

void pathDoc(xmlNode *root_node, List *pathList) {
    List *pathsList = pathList;
    ListIterator iterator = createIterator(pathsList);
    void *pathsElement;
    xmlNode *pathsNode;
    Path *paths;

    while ((pathsElement = nextElement(&iterator))) {
        paths = (Path *)pathsElement;
        pathsNode = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);

        xmlNewProp(pathsNode, BAD_CAST "d", BAD_CAST paths->data);

        if (paths->otherAttributes != NULL) {
            if (paths->otherAttributes->length) {
                otherAttrDoc(pathsNode, paths->otherAttributes);
            }
        }
    }
}

void groupDoc(xmlNode *root_node, List *groupList) {
    List *groupsList = groupList;
    ListIterator iterator = createIterator(groupsList);
    xmlNode *groupsNode;
    void *groupsElement;
    Group *groups;

    while ((groupsElement = nextElement(&iterator))) {
        groups = (Group *)groupsElement;
        groupsNode = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);

        if (groups->otherAttributes != NULL) {
            if (groups->otherAttributes->length) {
                otherAttrDoc(groupsNode, groups->otherAttributes);
            }
        }

        //Adding to doc in the order: rect, circle, path and then group 
        if (groups->rectangles != NULL) {
            if (groups->rectangles->length) {
                rectangleDoc(groupsNode, groups->rectangles);
            }
        }

        if (groups->circles != NULL) {
            if (groups->circles->length) {
                circleDoc(groupsNode, groups->circles);
            }
        } 
        
        if (groups->paths != NULL) {
            if (groups->paths->length) {
                pathDoc(groupsNode, groups->paths);
            }
        }

        if (groups->groups != NULL) {
            if (groups->groups->length) {
                groupDoc(groupsNode, groups->groups);
            }
        }
    }
}

// function to validate the XML doc against XSD
bool validateAgainstXSD(const char* schemaFile, xmlDoc *doc) {
    // Contains code from http://knol2share.blogspot.com/2009/05/validate-xml-against-xsdin-c.html provided by Prof. Nikitenko
    bool validate = false;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    xmlLineNumbersDefault(1);

    ctxt = xmlSchemaNewParserCtxt(schemaFile);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    int numOfErrors = 0;
    xmlSchemaValidCtxtPtr eCtxt;

    if (doc != NULL) {
        eCtxt = xmlSchemaNewValidCtxt(schema);
        
        xmlSchemaSetValidErrors(eCtxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        numOfErrors = xmlSchemaValidateDoc(eCtxt, doc);

        if (numOfErrors == 0) {
            validate = true;
        }

        xmlSchemaFreeValidCtxt(eCtxt);

        // free the schema file
        if(schema != NULL) {
		    xmlSchemaFree(schema);
        }
        xmlSchemaCleanupTypes();
    }
    return validate;
}

// function to validate the image against the constraints given
bool checkImage(const SVG *img) {
    //Header constraint checks
    if (!validateRects(img->rectangles) || !validateCircles(img->circles) ||
        !validatePaths(img->paths) || !validateGroups(img->groups) || 
        !validateAttributes(img->otherAttributes) || !validateSvgContraints(img)) {
        
        return false;
    }

    return true;
}

// function to validate rectangle lists against the constraints given
bool validateRects(List* rectangleList) {
    if (rectangleList == NULL) {
        return false;
    }

    ListIterator iterator = createIterator(rectangleList);
    Rectangle* rect = NULL;

    while ((rect = nextElement(&iterator)) != NULL) {
        if (rect->units == NULL || rect->otherAttributes == NULL || !validateAttributes(rect->otherAttributes)) {
            return false;
        } else if (rect->width < 0 || rect->height < 0) {
            return false;
        }
    }
    return true;
}

// function to validate other attributes against the constraints given
bool validateAttributes(List* otherAttrList) {
    if (otherAttrList == NULL) {
        return false;
    }

    ListIterator iterator = createIterator(otherAttrList);
    Attribute* attr = NULL;

    while ((attr = nextElement(&iterator)) != NULL) {
        if (attr->name == NULL || attr->value == NULL || attr->name == NULL ) {
            return false;
        }
    }
    return true;
}

// function to validate circle lists against the constraints given
bool validateCircles(List* circleList) {
    if (circleList == NULL) {
        return false;
    }

    ListIterator iterator = createIterator(circleList);
    Circle* circle = NULL;

    while ((circle = nextElement(&iterator)) != NULL) {
        if (circle->units == NULL || circle->otherAttributes == NULL || !validateAttributes(circle->otherAttributes)) {
            return false;
        } else if (circle->r < 0) {
            return false;
        }
    }
    return true;
}

// function to validate path lists against the constraints given
bool validatePaths(List* pathList) {
    if (pathList == NULL) {
        return false;
    }

    ListIterator iterator = createIterator(pathList);
    Path* path = NULL;

    while ((path = nextElement(&iterator)) != NULL) {
        if (path->data == NULL || path->otherAttributes == NULL || !validateAttributes(path->otherAttributes)) {
            return false;
        }
    }
    return true;
}

// function to validate group lists against the constraints given
bool validateGroups(List* groupList) {
    if (groupList == NULL) {
        return false;
    }

    ListIterator iterator = createIterator(groupList);
    Group* group = NULL;

    while ((group = nextElement(&iterator)) != NULL) {
        if (!validateRects(group->rectangles) || !validateCircles(group->circles) ||
            !validatePaths(group->paths) || !validateGroups(group->groups) ||
            !validateAttributes(group->otherAttributes)) {
            
            return false;
        }
    }
    return true;
}

// function to validate svg struct elements against the constraints given
bool validateSvgContraints(const SVG *img) {
    if (img->rectangles == NULL || img->circles == NULL || img->paths == NULL || 
        img->groups == NULL || img->otherAttributes == NULL) {

        return false;
    }
    
    if (img->namespace == NULL || img->title == NULL || img->description == NULL) {
        return false;
    }

    return true;
}

bool setSvgAttr(SVG* img, Attribute* newAttribute) {
    bool success = false;

    // setting the namespace
    if (strcmp(newAttribute->name, "xmlns") == 0) {
        if (newAttribute->value == NULL || strlen(newAttribute->value) == 0) {
            return false;

        } else {
            strcpy(img->namespace, newAttribute->value);
            free(newAttribute);
            success = true;
            return true;
        }

    // setting description
    } else if (strcmp(newAttribute->name, "desc") == 0) {
        if (newAttribute->value == NULL) {
            return false;

        } else {
            strcpy(img->description, newAttribute->value);
            free(newAttribute);
            success = true;
            return true;
        }

    // setting title
    } else if (strcmp(newAttribute->name, "title") == 0) {
        if (newAttribute->value == NULL) {
            return false;

        } else {
            strcpy(img->title, newAttribute->value);
            free(newAttribute);
            success = true;
            return true;
        }

    // looking in the otherAttributes 
    } else {
        success = setOtherAttr(img->otherAttributes, newAttribute);
    }

    return success;
}

bool setCircleAttr(SVG* img, int elemIndex, Attribute* newAttribute) {
    
    ListIterator iterator = createIterator(img->circles);
    Circle* circ = nextElement(&iterator);
    int index = 0;
    bool success = false;

    while (circ != NULL) {
        // if this is the circle 
        if (index == elemIndex) {
            // if the attribute name is cx
			if(strcmp(newAttribute->name, "cx") == 0) {
				circ->cx = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute name is cy
			} else if(strcmp(newAttribute->name, "cy") == 0) {
				circ->cy = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute name is r
			} else if(strcmp(newAttribute->name, "r") == 0) {
				circ->r = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute is located in other attributes
			} else {
                success = setOtherAttr(circ->otherAttributes, newAttribute);
                return success;
            } 
        }
        index++;
        circ = nextElement(&iterator);
    }
    return success;
}

bool setOtherAttr(List *otherAttr, Attribute* newAttribute) {
    if (otherAttr == NULL) {
        return false;
    }

    ListIterator iterator = createIterator(otherAttr);
    Attribute* attr = nextElement(&iterator);
    bool success = false;

    if (otherAttr->length > 0) {
        while (attr != NULL) {
            // if the attribute is in otherAttributes List, change the value
            if (strcmp(newAttribute->name, attr->name) == 0) {
                success = true;

                strcpy(attr->value, newAttribute->value);
            }
            attr = nextElement(&iterator);
        }  
    }

    // if the attribute is not in otherAttributes List, add it to the end
    if (!success) {
        insertBack(otherAttr, newAttribute);
        success = true;
    } else {
        deleteAttribute(newAttribute);
    }
    return success;
}

bool setRectangleAttr(SVG* img, int elemIndex, Attribute* newAttribute) {
    
    ListIterator iterator = createIterator(img->rectangles);
    Rectangle* rect = nextElement(&iterator);
    int index = 0;
    bool success = false;

    while (rect != NULL) {
        // if this is the circle 
        if (index == elemIndex) {
            // if the attribute name is x
			if(strcmp(newAttribute->name, "x") == 0) {
				rect->x = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute name is y
			} else if(strcmp(newAttribute->name, "y") == 0) {
				rect->y = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute name is width
			} else if(strcmp(newAttribute->name, "width") == 0) {
				rect->width = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;

            // if the attribute name is height
			} else if(strcmp(newAttribute->name, "height") == 0) {
				rect->height = atof(newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute is located in other attributes
			} else {
                success = setOtherAttr(rect->otherAttributes, newAttribute);
                return success;
            }    
        }
        index++;
        rect = nextElement(&iterator);
    }
    return success;
}

bool setPathAttr(SVG* img, int elemIndex, Attribute* newAttribute) {
    
    ListIterator iterator = createIterator(img->paths);
    Path* paths = nextElement(&iterator);
    int index = 0;
    bool success = false;

    while (paths != NULL) {
        // if this is the circle 
        if (index == elemIndex) {
            // if the attribute name is x
			if(strcmp(newAttribute->name, "d") == 0) {
                strcpy(paths->data, newAttribute->value);
				deleteAttribute(newAttribute);
				return true;
            // if the attribute is located in other attributes
			} else {
                success = setOtherAttr(paths->otherAttributes, newAttribute);
                return success;
            } 
        }
        index++;
        paths = nextElement(&iterator);
    }
    return success;
}

bool setGroupAttr(SVG* img, int elemIndex, Attribute* newAttribute) {
    ListIterator iterator = createIterator(img->groups);
    Group* groups = nextElement(&iterator);
    int index = 0;
    bool success = false;

    while (groups != NULL) {
        
        if (index == elemIndex) {
            
			if(groups->otherAttributes != NULL) {
                success = setOtherAttr(groups->otherAttributes, newAttribute);
                return success;
            }
        }
        index++;
        groups = nextElement(&iterator);
    }
    return success;
}

char* createSVGlogs(char* filename) {
    // This function will create a svg doc from a filename
    // It will then validate so only valid files can be used
    // Then it will convert it to a string before returning
    // char *file = malloc(sizeof(char) + 500);
    // strcpy(file, "uploads/");
    // strcat(file, filename);

    SVG *img = createValidSVG(filename, "parser/bin/svg.xsd");

    if (img == NULL) {
        return NULL;
    }  
    
    char* val = SVGtoJSON(img);

    if (img != NULL) {
        deleteSVG(img);
    }

    // free(file);

    return val;
}

char* fillSVGViewPanel(char* filename) {
    // This function will create a svg doc from a filename
    // It will then validate so only valid files can be used
    // Then it will convert it to a string before returning

    SVG *img = createValidSVG(filename, "parser/bin/svg.xsd");
    if (img == NULL) {
        return NULL;
    } 

    char* pathList = pathListToJSON(img->paths);
	char* groupList = groupListToJSON(img->groups);
	char* rectList = rectListToJSON(img->rectangles);
	char* circleList = circListToJSON(img->circles);
	char* attrList = attrListToJSON(img->otherAttributes);
    
    char* val = malloc(strlen(groupList) + strlen(pathList) + strlen(rectList) + strlen(circleList) + strlen(attrList) + 500);
    // strlen(groupList) + strlen(pathList) + strlen(rectList) + strlen(circleList) + strlen(attrList) 

    strcpy(val, "[");
    strcat(val, pathList);
    strcat(val, ",");
    strcat(val, groupList);
    strcat(val, ",");
    strcat(val, rectList);
    strcat(val, ",");
    strcat(val, circleList);
    strcat(val, ",");
    strcat(val, attrList);
    strcat(val, "]");

    // if (groupList != NULL) {
    //     free(groupList);
    // }
    // if (pathList != NULL) {
    //     free(pathList);
    // }
    // if (rectList != NULL) {
    //     free(rectList);
    // }
    // if (circleList != NULL) {
    //     free(circleList);
    // }
    // if (attrList != NULL) {
    //     free(attrList);
    // }
    if (img != NULL) {
        deleteSVG(img);
    }

    return val;
}

char* imgTitle(char *filename) {
	SVG *img = createValidSVG(filename, "parser/bin/svg.xsd");

	char* title = malloc(sizeof(char) + 500);
	strcpy(title, img->title);

	return title;
}

char* imgDescription(char *filename) {
	SVG *img = createValidSVG(filename, "parser/bin/svg.xsd");

	char* desc = malloc(sizeof(char) + 500);
	strcpy(desc, img->description);

	return desc;
}

void editTitleDesc(char* filename, char* title, char* description) {
    char *file = malloc(sizeof(char) + 500);
    strcpy(file, "uploads/");
    strcat(file, filename);
	SVG* img = createValidSVG(file, "parser/bin/svg.xsd");
	strcpy(img->title, title);
    img->title[255] = '\0';
	strcpy(img->description, description);
    img->description[255] = '\0';

	/*This will write it to a file*/
	bool valid = writeSVG(img, file);
	if(valid == false) {
        return;
    }

	deleteSVG(img);
    free(file);
}

int createNewSVGfromInput(char* filename, char* title, char* description) {
    SVG* img = createSVGImage();

    if (img == NULL) {
        return 0;
    }

    // Adding the namespace, title, description
    strcpy(img->namespace, "http://www.w3.org/2000/svg");
    strcpy(img->title, title);
    img->title[255] = '\0';
	strcpy(img->description, description);
    img->description[255] = '\0';

    if(validateSVG(img, "parser/bin/svg.xsd")) {
        writeSVG(img, filename);
    }

	deleteSVG(img);

    return 1;
}