#!/bin/sh

gcc --static -g -o UserBranchPatch UserBranchPatch.c ARMBranchPatch.c ThumbBranchPatch.c
