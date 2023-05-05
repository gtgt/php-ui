#!/usr/bin/env php74
<?php //phpcs:disable
	

	
#	;set_include_path(getenv('VPHP_LIBRARIES_ROOTS'))
	;require_once('ui.php')
	
	;define('ERROR_HANDLER_EXCEPTION_TRACE_ARGS_FORCE',true)
	;require_once('exc.php')
	;$ERROR_HANDLER_TRACE=true
	;$DEBUG_BACK_PEEK=10
	;$DEBUG_D_NS_MAP['b']=\com\dyn_o_saur\bluelephant::class
	
	;									ui\Window::fs(getenv('HOME').'/.v/ui')
	
	;									UI\onShouldQuit(function(){return(true);})
	
	;
	

	
	;use	\com\dyn_o_saur\bluelephant as b
	;use	\com\dyn_o_saur\bluelephant\ui
	;																		$area
	=									ui\area		(		20
		,								ui\fillArea		(								0x111111FF)
		,								ui\cross		(	10,null,0,	[		null],	0x8080FFFF)
		,...							ui\crossWithAL	(	10,null,0										#	radius	point	radianShift
			,															['Color'	=>	0x8080FFFF]			#				attrs
			,															[	[	null,'graduation'],	null	#		 layouter-	-leveledSubFr.
				,															[2,2]							#					-leveledSubFr:s
				,																null						#		graduator-
				,											10										,.15	#			-base	-breadth
				,																					null	#					-positioning
				,																					null	#					-layoutArgs
				,															['y','x','y','x']			]	#					-doubleRadianToLabel
			,															[	[	null,'graduation'],	null	#			liner-	-leveledSubFr.
				,															[2,2]							#					-leveledSubFr:s
				,																null				,.05]	#		graduator-	-breadth
			,																			0x8080FFFF)
		,...[							ui\pl()
				->								lines
					(	[							[-1		,	-4]
						,							[ 1		,	-1]
						,							[12.5	,	 2]])
				->								stroke(									0x80FFFFFF)
			,							ui\drawing(function($p,$cp	,$cz
					,												 $az,	$area){
					;										$p
						->						write
						(				ui\point(	[ 5		,	 5])
						,				ui\attributed(''.'['.'['.	'$az'
							.' = '.		json_encode					($az)
							.' / '.									'$cz'
							.' = '.		json_encode					($cz).']'
							. ' ' .									'@wz'
							.' = '.		json_encode
								(		ui\toArray						(	$area->getWindow()->getSize())).']'
							,										 $cz[0])
						->						attr					('Color',		0x00FF00FF)
						());})
			])
	;													$scptName
	=													$scptId='phlot' // cspell:ignore phlot
	;									ui\menu		(	$scptName)
	;									ui\window	(	$scptId
		,											[640	, 480 ] // default
		,									true
		,								ui\Box::Vertical	,false
		,								ui\box
			(							ui\Box::Horizontal	,false
			,																$area->scaleCombo		()
			,																$area->transformCheck	()
			)
		->								 setPadded			(true)
		,																	$area)
	->									 setTitle	(	$scptName)
	->										showAndRun()
	

	
	?>