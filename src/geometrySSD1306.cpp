/*
*
*   Created on: 29/03/2022
*   Author: Mario Paja
*
* 
*/

#include "SSD1306.h"
#include "mbed.h"
#include "Font.h"

void SSD1306::drawLine (char xStart, char yStart, char xEnd, char yEnd, printMode mode, bool refresh){
    int dx =  abs (xEnd - xStart), sx = xStart < xEnd ? 1 : -1;
    int dy = -abs (yEnd - yStart), sy = yStart < yEnd ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while(true) {
        /* loop */
        printPixel (xStart, yStart, mode, false);
        if (xStart == xEnd && yStart == yEnd) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;    /* e_xy+e_x > 0 */
            xStart += sx;
        }
        if (e2 <= dx) {
            err += dx;    /* e_xy+e_y < 0 */
            yStart += sy;
        }
    }
    if (refresh)
        refreshDisplay();
}
