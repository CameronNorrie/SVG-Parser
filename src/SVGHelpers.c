#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVGParser.h"
#include "SVGHelpers.h"
#include <math.h>

/*
 *  CIS*27500 Assignment #1
 *  author: Cameron Norrie (1046856)
 *  Project: SVGHelpers.c
 *  Date: February 4, 2022
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

    int counter = 0;
    int startCopy = 0;
    int length = strlen(searchContent);
    char temp[length + 1];
    strcpy(temp, searchContent);
    temp[length] = '\0';

    //find where the units start
    for (int index = 0; index < length; index++){
        if (isalpha(searchContent[index]) != 0 && index != length) {
            startCopy = index;
            break;
        }
    }
    // place the units into the unit array
    for (int i = startCopy; i < length; i++) {
        unitChar[counter] = temp[i];
        counter++;
    }
    // null terminate end of the unit
    unitChar[counter] = '\0';
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
            addUnits(rectangles->units, (char *)(value->content));

        } else if (strcmp(attrName, "y") == 0) {       
            float temp;

            temp = atof((char*)cont);
            rectangles->y = temp;
            addUnits(rectangles->units, (char *)(value->content));

        } else if (strcmp(attrName, "width") == 0) {     
            float temp;

            temp = atof((char*)cont);
            rectangles->width = temp;
            addUnits(rectangles->units, (char *)(value->content));

        } else if (strcmp(attrName, "height") == 0) {       
            float temp;

            temp = atof((char*)cont);
            rectangles->height = temp;
            addUnits(rectangles->units, (char *)(value->content));

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
            addUnits(circles->units, (char *)(value->content));

        } else if (strcmp(attrName, "cy") == 0) {       
            float temp;

            temp = atof((char*)cont);
            circles->cy = temp;
            addUnits(circles->units, (char *)(value->content));

        } else if (strcmp(attrName, "r") == 0) {     
            float temp;

            temp = atof((char*)cont);
            circles->r = temp;
            addUnits(circles->units, (char *)(value->content));

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

    Path *path = malloc(sizeof(Path) + length* sizeof(char) + 5);
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
