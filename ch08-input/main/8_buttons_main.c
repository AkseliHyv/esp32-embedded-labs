#include <stdint.h>
typedef struct {
    char name[6];
    int pin;
    uint32_t bit;
} button;

static button buttons[8] = 
{
    {"UP", 4, 1 << 0}, {"DOWN", 5, 1 << 1}, {"LEFT", 6, }, 
    {7, }, {}, {}, 
    {}, {}
};