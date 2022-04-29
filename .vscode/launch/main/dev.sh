#!/usr/bin/env bash

	
	:			;((!full_env																															))||{
		:		;. "$(	vpath vbash base																												)"	;	# for _vd #cspell:ignore vpath
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;					  init=( "${ARGV2[@]}")																									;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;	((						 ${#ARGV2[@]}																								))	\
		&&						   phpargs=( "${ARGV2[@]}")																									\
		||						   phpargs=(																											
						-d extension=$(dirname "$0")/modules/ui.so																						)	;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;					before=( "${ARGV2[@]}")																									;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;					inArea=( "${ARGV2[@]}")																									;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;	((						 ${#ARGV2[@]}																								))	\
		&&						beforeBox=(	 "${ARGV2[@]}")																									\
		||						beforeBox=(																												)	;
		:		;. "$(	vpath vbash include		argument double-dash slurp																				)"	;
		:		;	((						 ${#ARGV2[@]}																								))	\
		&&						 afterBox=(	 "${ARGV2[@]}")																									\
		||						 afterBox=(																												)	;
		:		;:																																			;}	;
	
	:			;	C=(	vphp3.8	   "${init[@]}"																												#cspell:ignore vphp
				--				"${phpargs[@]}"																												
				--		-Ep																																	
					-\;	-r	ui	 "${before[@]}"																												
					
					-\;
							 -\$area																												-=		
						-d	ui\\area								-\(		  20																		
						-d	ui\\fillArea								-\(										0x111111FF							-\)	
						-d	ui\\cross									-\(	  10				-ø				 0									
																						-[		-ø			-]	0x8080FFFF							-\)	
#						-d	ui\\cross									-\(	  10				-ø				 0									
#									-[	 -[ -ø graduation			-]							-ø													#			liner-	-leveledSubFr.
#																						-[	 2		 2		-]										#					-leveledSubFr:s
#																								-ø				.05								-]	#		graduator-	-breadth
#																												0x8080FFFF							-\)	
						-,... -+
						-d	ui\\crossWithAL								-\(	  10				-ø				 0									#	radius	point	radianShift
									-[										Color				-=				0x8080FFFF						-]	#				attrs
									-[	 -[ -ø graduation			-]							-ø													#		 layouter-	-leveledSubFr.
																						-[	 2		 2		-]										#					-leveledSubFr:s
																								-ø													#		graduator-
#																			  10								.2									#			-base	-breadth
																			  10								.15									#			-base	-breadth
																								-ø													#					-positioning
																								-ø													#					-layoutArgs
																						-[	-\'{,}{y,x}		-]									-]	#					-doubleRadianToLabel
									-[	 -[ -ø graduation			-]							-ø													#			liner-	-leveledSubFr.
																						-[	 2		 2		-]										#					-leveledSubFr:s
																								-ø				.05								-]	#		graduator-	-breadth
																												0x8080FFFF							-\)
						-,... -+[ # circumventing PHP Fatal error:  Cannot use positional argument after argument unpacking
						-d	ui\\pl\(\)	-' -> 'lines					-\([			-[	–1		–4		-]								
																						-[	 1		–1		-]								
																						-[	 12.5	 2		-]								-]	-\)	
						-+				-' -> 'stroke					-\(										0x80FFFFFF						-\)	
						-d	ui\\drawing							-\(	-a\(\$p$(															
							),$(									   )\$cp$(															
							),$(									   )\$cz$(															
							),$(									   )\$az$(															
							),\$area$(																									)\)	-\{
																	-_	\$p																	
										-' -> 'write				-\(																	
						-d	ui\\point									-\(				-[	 5		 5		-]						-\)	
						-d	ui\\attributed								-\(	''														
						-.				 '['																						
						-.				 '['						-.	\$az														
						-.				' = '						-.	-_	json_encode											-\(
																	-_	\$az													-\)	
						-.				' / '						-.	\$cz														
						-.				' = '						-.	-_	json_encode											-\(
																	-_	\$cz													-\)	
						-.				 ']'																						
						-.				 ' '						-.	\@wz														
						-.				' = '						-.	-_	json_encode											-\(
						-+d	ui\\toArray																						-\( # CHECK to remove + opt here
							 -\$area	-' -> 'getWindow				-\(												-\)	
										-' -> 'getSize					-\(												-\)	-\)	-\)	# size is inner (ie sans macos 10.13 titlebar)
						-.				 ']'																						
																	-,	\$az[0]														-\)	
										-' -> 'attr						-\(	Color								0x00FF00FF			-\)	
																		-\(															-\)	
																																		-\)	-\}	-\)	
								 "${inArea[@]}"																										-]	-\)	
					
					-\;	-d	ui\\Window::fs					-\(				"$(fs=~/.v/ui && ([[ -d "$fs" ]]||mkdir "$fs") && printf %s "$fs")"			-\)	
					-\;		UI\\onShouldQuit				-\(		-a\{	return								-,	true						-\;	-\}	-\)	
					-\;														-\$scptName																		
					-=														-\$scptId -= phlot																
					-\; -d	ui\\menu						-\(				-\$scptName																	-\)	
					-\;	-d	ui\\window						-\(				-\$scptId	-[	640		480		-]	-,	true								# on macOS quiting from app menu or dock - neither saves frame - wh/as clicking the window-close-button does, hm..
#																												-,	null	-,	false					
						-d	ui\\Box::Vertical																				-,	false					
							  "${beforeBox[@]}"																											
						-d	ui\\box							-\(																						
						-d	ui\\Box::Horizontal																				-,	false				
						-,	''																														
						   -\(																													
							 -\$area																											-\)	
						-+				-' -> '$( )scaleCombo\($(																				)\)	
							 -\$area	-' -> 'transformCheck\($(																				)\)	
																																					-\)	
						-+				-' -> 'setPadded	-\(																-,	true				-\)	
							 -\$area																													
#							 -\$area' -> 'square_aspectCheck\($(																					)\)	
							   "${afterBox[@]}"																											-\)	
						-+				-' -> 'setTitle		-\(			-\$scptName																		-\)	
						-+				-' -> 'showAndRun	-\(																							-\)	
#						-v																																	
						"$@"																																
																																							)	;
	
