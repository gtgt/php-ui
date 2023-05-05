#!/usr/bin/env bash

	
	:			;((!full_env																															))||{
		:		;. "$(	vpath vbash base																												)"	;	# for _vd #cspell:ignore vpath
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;							  init=( "${ARGV2[@]}")																							;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;	((						 ${#ARGV2[@]}																								))	\
		&&								   phpargs=( "${ARGV2[@]}")																							\
		||								   phpargs=(																									
						-d extension=$(dirname "$0")/modules/ui.so																						)	;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;							before=( "${ARGV2[@]}")																							;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;						  subjects=( "${ARGV2[@]}")																							;
		:		;:																																			;}	;
	
	:			;	C=(	vphp3.8			   "${init[@]}"																										#cspell:ignore vphp
				--						"${phpargs[@]}"																										
				--		-Ep																																	
					-\;	-r		phlot	 "${before[@]}"																										
					
					-\;	-d	ui\\Window::fs					-\(				"$(fs=~/.v/ui && ([[ -d "$fs" ]]||mkdir "$fs") && printf %s "$fs")"			-\)	
					-\;		UI\\onShouldQuit				-\(		-a\{	return								-,	true						-\;	-\}	-\)	
					
					-\;	-d	ui\\phlot								-\(							1														
																			-[			-[	–1		–4		-]										
																						-[	 1		–1		-]										
																						-[	 12.5	 2		-]										-]	-\)	
#						-v																																	
						"$@"																																
																																							)	;
	
