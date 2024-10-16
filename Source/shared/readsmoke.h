#ifndef READSMOKE_H_DEFINED
#define READSMOKE_H_DEFINED
#include "options.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MALLOCC.h"
#include "contourdefs.h"
#include "histogram.h"
#include "isodefs.h"
#include "shared_structures.h"
#include "smokeviewdefs.h"
#include "string_util.h"

#include "readobject.h"

void FreeSmoke3D(smoke3ddata *smoke3di);
#endif