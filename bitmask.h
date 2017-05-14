/*************************************************************************************//**
 *  @file       bitmask.h
 *
 *  @brief      Brief description of bitmask.h
 *
 *  @date       2017-04-27 18:08
 *
 **************************************************************************************/


#ifndef BITMASK_H
#define BITMASK_H


#define BIT(x) (0x01 << (x))

#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) (((x) & (y)) == (y))



#endif /* !BITMASK_H */

