	;&	{												$commandName=&{																			`
			;					function	unless(		$subject,			$isReplacedBy													)	# function should sync w/ ~vike/.config/powershell/base.ps1
				{if(									$subject -notMatch	$isReplacedBy[0]){throw}										;`
				;return									$subject -replace	$isReplacedBy													;}	`
			;	(							unless	(	get-PSCallStack																	`
						)[0].scriptName					-isReplacedBy '-Register-ArgumentCompleter(\.[^.]+)$','$1'							)	;}	;
#		;				write-host					(	$commandName																			)	;
		;Register-ArgumentCompleter																												`
						   -CommandName					$commandName -ScriptBlock{																`
			 param(																				$word , $line , $linePos					)	;
#			;$n	=	eval(d('																	$word','$line','$linePos				')	)	; # req ~vike/.config/powershell/base.ps1
			;											$commandName=&{																		`
				;				function	unless(		$subject,			$isReplacedBy												)	# function should sync w/ ~vike/.config/powershell/base.ps1
					{if(								$subject -notMatch	$isReplacedBy[0]){throw}									;
					;return								$subject -replace	$isReplacedBy												;}	`
				;	(						unless	(	get-PSCallStack																`
						)[0].scriptName					-isReplacedBy '-Register-ArgumentCompleter(\.[^.]+)$','$1'						)	}	;
#			;			write-host					(	$commandName)																			;
			;																	 $script:argumentComplete			=	$false					;
			;		$soft																														`
			=	(		get-command						$commandName																		).	`
							 scriptBlock.AST.findAll(	{	if(	$args[0].	name -eq	'argumentCompleteEnd'		){				`
						;														 $script:argumentComplete			=	$false		`
						;																								$false		}
					 									elseif(	$args[0].	name -eq	'argumentCompleteBegin'		){				`
						;														 $script:argumentComplete			=	$true		`
						;																								$false		}
					 									elseif(					 $script:argumentComplete			){				`
						;										$args[0] -is [Management.Automation.Language.FunctionDefinitionAst]	}		# https://stackoverflow.com/questions/59993421/is-there-a-way-to-show-all-functions-in-a-powershell-script/59995127#59995127
					},																									$false				)	| # last $false here is "`searchNestedScriptBlocks` Search nested functions and script block expressions."
			 				foreach-object				{				$_.	name															}	;
			;&	{	$soft|	foreach-object				{'force-'	+	$_																}	;
				;	$soft|	foreach-object				{				$_																}	}	|
			 				  where-object				{				$_.		substring(0,	$word.length)								`
				 																			-eq	$word										}	;}	}