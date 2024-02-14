#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVGParser.h"
#include "SVGHelpers.h"
#include <math.h>

/* DON'T SUBMIT THIS FILE */

int main ()
{
    SVG *img = NULL;
    // printf("hello world\n");
    // img = createSVGimage("MySVGs/Emoji_poo.svg"); // Testing my own SVGs
    // img = createSVG("quad01.svg");

    // img = createValidSVG("Emoji_poo.svg", "svg.xsd");
    img = createValidSVG("quad01.svg", "svg.xsd");

    // img = JSONtoSVG("{\"title\":\"titleVal\",\"descr\":\"descrVal\"}");

    // Rectangle* rect = createRectangle();
    // JSONtoRect("{\"x\":1,\"y\":2,\"w\":19,\"h\":15,\"units\":\"cm\"}");

    // printf("%s\n", img->title);
    // printf("%s\n", img->description);

    // printf("%f\n", rect->x);
    // printf("%f\n", rect->y);
    // printf("%f\n", rect->width);
    // printf("%f\n", rect->height);
    // printf("%s\n", rect->units);

    // deleteRectangle(rect);
    // free(rect);
    
    // void *circElem;
    // Circle *circNode;
    // ListIterator iterate = createIterator(img->circles);
    // while((circElem = nextElement(&iterate))) {
    //     circNode = (Circle *)circElem;

    //     freeList(circNode->otherAttributes);
    //     circNode->otherAttributes = NULL;
    // }

    // bool validImage = validateSVG(img, "svg.xsd");
    // if (validImage) {
    //     printf("SVGimage is valid\n");
    // } else {
    //     printf("SVGimage is invalid\n");
    // }
    
    // char *string = SVGimageToString(img);
    // printf("Printing SVG Image in MAIN:\n%s\n", string);

    // free(string);

    // printf("TESTING writeSVGimage\n");
    // // if (writeSVGimage(img, NULL)) { // WHERE IT SAYS NULL, IT SHOULD PASS A FILENAME!!!!
    //     if (writeSVG(img, "Cameron.svg")) { // WHERE IT SAYS NULL, IT SHOULD PASS A FILENAME!!!!
    //         printf("writeSVG is true\n");
    //     } else {
    //         printf("writeSVG is false\n");
    //     }

    /* printf("testing setAttr for SVG's other attr\n");
    Attribute *newAttr = calloc(1, sizeof(Attribute));
    newAttr->name =(char *)calloc(123, sizeof(char));
    newAttr->value =(char *)calloc(123, sizeof(char));
    // newAttr->name = "viewBox";
    // newAttr->value = "0 0 128 128";
    strcpy(newAttr->name, "viewBox");
    strcpy(newAttr->value, "0 0 128 128");
    Attribute *newAttrT= calloc(1, sizeof(Attribute)); // title
    Attribute *newAttrD= calloc(1, sizeof(Attribute)); // description
    Attribute *newAttrNS= calloc(1, sizeof(Attribute)); // namespace
    newAttrT->name =(char *)calloc(123, sizeof(char));
    newAttrT->value =(char *)calloc(123, sizeof(char));
    strcpy(newAttrT->name, "title");
    strcpy(newAttrT->value, "suck my ass");
    newAttrD->name =(char *)calloc(123, sizeof(char));
    newAttrD->value =(char *)calloc(123, sizeof(char));
    strcpy(newAttrD->name, "desc");
    strcpy(newAttrD->value, "shrek 3 is lovely film");
    newAttrNS->name =(char *)calloc(123, sizeof(char));
    newAttrNS->value =(char *)calloc(123, sizeof(char));
    strcpy(newAttrNS->name, "xmlns");
    strcpy(newAttrNS->value, "doodoo butter");
    setAttribute(img, SVG_IMAGE, 0, newAttr);
    setAttribute(img, SVG_IMAGE, 0, newAttrT);
    setAttribute(img, SVG_IMAGE, 0, newAttrD);
    setAttribute(img, SVG_IMAGE, 0, newAttrNS); */

    /* printf("testing setAttr for Circle's other attr\n");
    Attribute *newAttrC = calloc(1, sizeof(Attribute));
    Attribute *newAttrCO = calloc(1, sizeof(Attribute));
    newAttrCO->name =(char *)calloc(123, sizeof(char));
    newAttrCO->value =(char *)calloc(123, sizeof(char));
    newAttrC->name =(char *)calloc(123, sizeof(char));
    newAttrC->value =(char *)calloc(123, sizeof(char));
    // newAttrCO->name = "fill";
    // newAttrCO->value = "69";
    // newAttrC->name = "r";
    // newAttrC->value = "-1";
    strcpy(newAttrCO->name, "fill");
    strcpy(newAttrCO->value, "69");
    strcpy(newAttrC->name, "r");
    strcpy(newAttrC->value, "-1");
    setAttribute(img, CIRC, 0, newAttrCO);
    setAttribute(img, CIRC, 0, newAttrC); */ // First circle's other attr is gonna get changed, specifically its radius

    /* printf("testing setAttr for Rect's other attr\n");
    Attribute *newAttrR= calloc(1, sizeof(Attribute));
    Attribute *newAttrRO= calloc(1, sizeof(Attribute));
    newAttrRO->name =(char *)calloc(123, sizeof(char));
    newAttrRO->value =(char *)calloc(123, sizeof(char));
    newAttrR->name =(char *)calloc(123, sizeof(char));
    newAttrR->value =(char *)calloc(123, sizeof(char));
    // newAttrRO->name = "fill";
    // newAttrRO->value = "666";
    // newAttrR->name = "height";
    // newAttrR->value = "-1";
    strcpy(newAttrRO->name, "fill");
    strcpy(newAttrRO->value, "666");
    strcpy(newAttrR->name, "height");
    strcpy(newAttrR->value, "-1");
    // program seg faults!!!! if I remove fill attribute in rect.svg's rectangle
    setAttribute(img, RECT, 0, newAttrRO);
    setAttribute(img, RECT, 0, newAttrR); */

    /* printf("testing setAttr for Path's other attr\n");
    Attribute *newAttrP= calloc(1, sizeof(Attribute));
    Attribute *newAttrPO= calloc(1, sizeof(Attribute));
    newAttrPO->name =(char *)calloc(123, sizeof(char));
    newAttrPO->value =(char *)calloc(123, sizeof(char));
    newAttrP->name =(char *)calloc(123, sizeof(char));
    newAttrP->value =(char *)calloc(123, sizeof(char));
    // newAttrPO->name = "fill";
    // newAttrPO->value = "666";
    // newAttrP->name = "data";
    // newAttrP->value = "wowzerzzz";
    strcpy(newAttrPO->name, "fill");
    strcpy(newAttrPO->value, "666");
    strcpy(newAttrP->name, "data");
    strcpy(newAttrP->value, "wowzerzzz");
    setAttribute(img, PATH, 1, newAttrPO);
    setAttribute(img, PATH, 0, newAttrP); */

    /* printf("testing setAttr for Group's other attr\n");
    Attribute *newAttrG= calloc(1, sizeof(Attribute));
    Attribute *newAttrGO= calloc(1, sizeof(Attribute));
    newAttrGO->name =(char *)calloc(123, sizeof(char));
    newAttrGO->value =(char *)calloc(123, sizeof(char));
    strcpy(newAttrGO->name, "fill");
    strcpy(newAttrGO->value, "666");
    // newAttrGO->name = "fill";
    // newAttrGO->value = "666";
    newAttrG->name =(char *)calloc(123, sizeof(char));
    newAttrG->value =(char *)calloc(123, sizeof(char));
    strcpy(newAttrG->name, "data");
    strcpy(newAttrG->value, "wowzerzzz");
    // newAttrG->name = "data";
    // newAttrG->value = "wowzerzzz";
    setAttribute(img, GROUP, 0, newAttrGO);
    setAttribute(img, GROUP, 4, newAttrG); */

    /* printf("testing addComponent using Rect\n");
    Rectangle *rectToAdd = calloc(1, sizeof(Rectangle));
    rectToAdd->height=7.6;
    // rectToAdd->units=(char *)calloc(123, sizeof(char));
    strcpy(rectToAdd->units, "");
    rectToAdd->width=4.5;
    rectToAdd->x=4.3;
    rectToAdd->y=5.6;
    rectToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    Attribute *random1 = calloc(1, sizeof(Attribute));
    // Attribute random2;
    random1->name=(char *)calloc(123, sizeof(char));
    random1->value=(char *)calloc(123, sizeof(char));
    strcpy(random1->name, "Rect_test");
    strcpy(random1->value, "value");
    insertBack(rectToAdd->otherAttributes, random1);
    if (rectToAdd->otherAttributes == NULL) printf("HELLOWEWEW\n");
    addComponent(img, RECT, rectToAdd); */

    printf("testing attrToJSON\n");
    char *testStr = malloc(sizeof(char) + 10000);
    Attribute *testAttr = malloc(sizeof(Attribute) + 10000);
    testAttr->name = malloc(sizeof(char) +100000);
    sprintf(testAttr->name, "fill");
    sprintf(testAttr->value, "redDabberz");
    testStr = attrToJSON(testAttr);
    printf("attrToJSON returns: %s\n", testStr); // test using NULL for all _toJSON functions
    // deleteAttribute(testAttr);
    // free(testStr); 

    printf("testing circleToJSON\n");
    Circle *testCircle = malloc(sizeof(Circle) + 1000);
    testCircle->cx = 69.42057;
    testCircle->cy = 69.420;
    testCircle->r = 5.6;
    testCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    // insertBack(testCircle->otherAttributes, testAttr);
    sprintf(testCircle->units, "cool");
    testStr = circleToJSON(testCircle);
    printf("circleToJSON returns: %s\n", testStr);
    // deleteCircle(testCircle);
    // deleteAttribute(testAttr);
    // free(testStr); 

    printf("testing rectToJSON\n");
    Rectangle *testRect = malloc(sizeof(Rectangle) + 10000);
    testRect->x = 69.42057;
    testRect->y = 69.420;
    testRect->width = 5.6;
    testRect->height = 6.9;
    testRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    // insertBack(testRect->otherAttributes, testAttr);
    // sprintf(testRect->units, "weird");
    testStr = rectToJSON(testRect);
    printf("rectToJSON returns: %s\n", testStr);
    // deleteRectangle(testRect);
    // deleteAttribute(testAttr);
    // free(testStr); 

    printf("testing pathToJSON\n");
    Path *testPath = malloc(sizeof(Path) + 100000);
    sprintf(testPath->data, "'I kiss you on your forehead if you don't stop playing with me' - Kevin Gates");
    testPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    // insertBack(testPath->otherAttributes, testAttr);
    testStr = pathToJSON(testPath);
    printf("pathToJSON returns: %s\n", testStr);
    // deletePath(testPath);
    // deleteAttribute(testAttr);
    // free(testStr);

    printf("testing groupToJSON\n");
    Group *testGroup = malloc(sizeof(Group) + 1000);
    testGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    testGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);
    testGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);
    testGroup->paths = initializeList(pathToString, deletePath, comparePaths);
    testGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    insertBack(testGroup->paths, testPath);
    insertBack(testGroup->circles, testCircle);
    insertBack(testGroup->rectangles, testRect);
    insertBack(testGroup->otherAttributes, testAttr);
    testStr = groupToJSON(testGroup);
    printf("groupToJSON returns: %s\n", testStr);
    // deleteGroup(testGroup);
    // deleteAttribute(testAttr);
    // free(testStr);

    printf("testing SVGToJSON\n");
    testStr = SVGtoJSON(img);
    printf("SVGToJSON returns: %s\n", testStr);
    // free(testStr);

    printf("testing attrListToJSON\n");
    testStr = attrListToJSON(img->otherAttributes);
    printf("attrListToJSON returns: %s\n", testStr);
    // free(testStr);

    printf("testing circListToJSON\n");
    testStr = circListToJSON(img->circles);
    printf("circListToJSON returns: %s\n", testStr);
    // free(testStr);

    printf("testing rectListToJSON\n");
    testStr = rectListToJSON(img->rectangles);
    printf("rectListToJSON returns: %s\n", testStr); 
    // free(testStr);

    testStr = SVGToString(img);
    printf("%s", testStr);
    // free(testStr);

    // for JSONtoRect
    char *rectJSONStr = malloc(sizeof(char) + 10000);
    sprintf(rectJSONStr, "%s", testStr);

    // free(testStr);
    
    printf("testing pathListToJSON\n");
    testStr = pathListToJSON(img->paths);
    printf("pathListToJSON returns: %s\n", testStr);
    // free(testStr);
    
    printf("testing groupListToJSON\n");
    testStr = groupListToJSON(img->groups);
    printf("groupListToJSON returns: %s\n", testStr);
    free(testStr);

    printf("testing JSONtoRect\n");
    Rectangle* rect = createRectangle();
    rect = JSONtoRect(rectJSONStr);

    printf("%s\n", img->title);
    printf("%s\n", img->description);

    printf("%f\n", rect->x);
    printf("%f\n", rect->y);
    printf("%f\n", rect->width);
    printf("%f\n", rect->height);
    printf("%s\n", rect->units);

    deleteRectangle(rect);

    bool validImage = validateSVG(img, "svg.xsd");
    if (validImage) {
        printf("validateSVG is true\n");
    } else {
        printf("validateSVG is false\n");
    }

    if (validImage) {
    // if (writeSVGimage(img, NULL)) { // WHERE IT SAYS NULL, IT SHOULD PASS A FILENAME!!!!
        if (writeSVG(img, "Cameron.svg")) { // WHERE IT SAYS NULL, IT SHOULD PASS A FILENAME!!!!
            printf("writeSVG is true\n");
        } else {
            printf("writeSVG is false\n");
        }
    }

    // printf("testing addComponent function\n");
    // addComponent(img, , NULL);

    // stringSVG = NULL;

    
    deleteSVG(img);

    return 0;
}