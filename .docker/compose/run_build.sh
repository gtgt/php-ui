#!/usr/bin/env bash
	set -e
	

	
	phpize
	./configure
	make clean
	make
	
