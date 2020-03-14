#pragma once

#include <cmath>
#include <cstdlib>

namespace prl {
    int node_dist;

    float dotproduct(float _x, float _y, float xn, float yn, float gx, float gy) {
        float x = abs(xn-((float)_x)/node_dist);
        float y = abs(yn-((float)_y)/node_dist);
        return x*gx + y*gy;
    }
    
    float smoothstep(int _x) {
        float x = ((float)_x)/node_dist;
        return 6*pow(x,5)-15*pow(x,4)+10*pow(x,3);
    }
    
    float interpolate(int x, float a0, float a1) {
        return a0+smoothstep(x)*(a1-a0);
    }
    
    float perlin(float g[2][2][2], int x, int y) {
        int x0 = x/node_dist;
        int x1 = x/node_dist+1;
        
        int y0 = y/node_dist;
        int y1 = y/node_dist+1;
    
        float dp1 = dotproduct(x, y, x0, y0, g[x0][y0][0], g[x0][y0][1]);
        float dp2 = dotproduct(x, y, x0, y1, g[x0][y1][0], g[x0][y1][1]);
        float dp3 = dotproduct(x, y, x1, y0, g[x1][y0][0], g[x1][y0][1]);
        float dp4 = dotproduct(x, y, x1, y1, g[x1][y1][0], g[x1][y1][1]);
    
        float i1 = interpolate(x, dp1, dp3);
        float i2 = interpolate(x, dp2, dp4);
    
        float i3 = interpolate(y, i1, i2);
    
        return i3;
    }
}
