/*
main.c - The main entry point file for this entire project
    Copyright (C) 2025 bringupyourpost

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
#include "zip.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;

    unzip_sb3_to_cratch(argv[1]);
    return 0;
}
