#include "mpf.h"

enum class COLOR_TYPE {
    PRIMARY_BG, PRIMARY_FG,
    GRADIENT_ACC_1, GRADIENT_ACC_2
}

typedef struct COLOR {
    int r, g, b;

    COLOR(int r, int g, int b)
        : r(r), g(g), b(b);
} COLOR;

typedef std::map<COLOR_TYPE, COLOR> theme;


static theme DEFAULT = {
    {PRIMARY_BG: COLOR()},
};

static theme NORD = {
    // {PRIMARY_BG: },
};

static theme GRUV = {
    // {PRIMARY_BG: },
};