	 [Diagnostics.CodeAnalysis.SuppressMessageAttribute('PSAvoidUsingCmdletAliases'	, '', Scope='Function', Justification="to use cd tyvm")] #https://docs.microsoft.com/en-us/powershell/utility-modules/psscriptanalyzer/overview#suppressing-rules
	 [Diagnostics.CodeAnalysis.SuppressMessageAttribute('PSUseApprovedVerbs'		, '', Scope='Function', Justification="snake is less read-/writable and camel doesn't align with space - if I could add verbs I wouldn't be as opposed")]
#	 [cmdletBinding()]
#	 [cmdletBinding(SupportsShouldProcess)]
	 [cmdletBinding(PositionalBinding=$false)]
	 param
		([Parameter(Position=1)][string[]	]	$actions
		,[Parameter()						]	$runBase			=	$HOME
		,[Parameter()						]	$runCode
		=	{									mkdir		-force			"/home/vscode/.v/ui"									|out-null	;
			;$var,								$path	=					 'PHP_LIBRARIES_ROOT'												`
				,														(	$args[0]+'/shell/library/php'									)	;	[Environment]::SetEnvironmentVariable($var,				$path)
			;$var										=					'VPHP_LIBRARIES_ROOTS'												;
			;foreach(							$path in					arrgs																`
																		(	$args[0]+'/shell/library/php/com/dyn_o_saur/bluelephant'		)	){	[Environment]::SetEnvironmentVariable($var
					,																																[Environment]::GetEnvironmentVariable($var)+(get-item	$path).fullName+';','Process')} #cspell:ignore sysepos vphp
			;eval(d('														$runCode'))
			;write-output																					"go with g; quit with q"			;
			;&														  'C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe'			`
			 php									-c					'/opt/local/etc/php74'													`
													-d	 extension_dir=C:\php\debug\ext															`
													-d	 extension$( )=C:\php\debug\ext\php_ui.dll												`
																		(	$args[0]+'/file/develop/git/ui/launch-task.php')					;}
		,[Parameter()][string[]				]	$resetKeys
		,[Parameter()][string[]				]	$setKeys
		,[Parameter()][switch				]	$forceActions
#		,[Parameter()][switch				]	$deepForceActions
		,[Parameter()						]	$command # php-sdk-command
		,[Parameter()][switch				]	$choco_install_force
		,[Parameter()][string[]				]	$choco_install_args	=@('install','-y','--stop-on-first-failure') # if this is empty the force action makes splatting yield $null
		,[Parameter()						]	$choco				= {'choco'+' '+($args -join ' ')|write-host;choco @args} # write-host as elevateBy currently doesn't forward -verbose
		,[Parameter()						]	$elevateBy			= {$a=@();$input|foreach-object{$a+=$_};$a -join ' '|write-verbose;$c,$a=$a;&$c @a} # $input cause-of most hybrid and compat w/ recommendation: {$a=@();$input|% { $a+=$_ };'gSudo'+' '+($a-join' ')|write-verbose;$c,$a=$a;invoke-gSudo $c $a} # was: i dunno why arg $a instead of @a but here's my dev-test: &{function cc{eval(d('$args'))|ign;echo 'c!' @args};$cc={eval(d('$args'))|ign;cc @args};function s{echo 's?';$c,$a=$args;eval(d('$c','$a'))|ign;&$c @a};$e={$a=@();$input|foreach-object{$a+=$_};eval(d('$a'))|ign;$c,$a=$a;eval(d('$c','$a'))|ign;&$c $a};function e{eval(d('$args'))|ign;$args|&$e};e $cc "test fest" text} #cspell:ignore gSudo
		,[Parameter(DontShow)				]	$bluelephant # eval(d…(…)) requires ~vike/.config/powershell/base.ps1 wh/ ~vike is of (fake) organisation bluelephant
		,[Parameter(DontShow)				]	$subShellKeys) #cspell:ignore dont
#	 DynamicParam	{return [System.Management.Automation.RuntimeDefinedParameterDictionary]::new()} # nothing of everything I tried here (~vike should have vcp-d backup), including calling it here, works as does: Register-ArgumentCompleter
#	 DynamicParam	{
#		;								$attributeCollection	=					[System.Collections.ObjectModel.Collection[System.Attribute]	]::new()
##		;								$attributeCollection.	Add(				[System.Management.Automation.ParameterAttribute				]@{Mandatory=$true}) # required for completion of/after dash -[[a]c] etc
#		;								$attributeCollection.	Add(				[System.Management.Automation.ParameterAttribute				]@{Position=1})
##		;								$attributeCollection.	Add(				[System.Management.Automation.ParameterAttribute				]@{Position=0;Mandatory=$true}) # Mandatory=$true: required for completion of/after dash -[[a]c] etc
##		;								$attributeCollection.	Add(				[System.Management.Automation.ArgumentCompleterAttribute		]::new({'test','text'|% {$_}}))
###		;								$attributeCollection.	Add(				[System.Management.Automation.ArgumentCompleterAttribute		]::new({@('test';'text')}))
#		;								$paramDictionary		=					[System.Management.Automation.RuntimeDefinedParameterDictionary	]::new()
#		;								$paramDictionary.		Add(																						'action' # why repeat our selves next you ask? cause we want it wet and sleazy (as opposed to DRY)! if this differs to below we brilliant get the message (btw fyi imo params should be coll instead dict): Cannot bind cmdlet ….ps1 because runtime-defined parameter '…' was added to the RuntimeDefinedParameterDictionary with key '…'. The key must be the same as RuntimeDefinedParameter.Name.
#			,																		[System.Management.Automation.RuntimeDefinedParameter			]::new(	'action', [object]
#			,							$attributeCollection))
#		;return							$paramDictionary
#		}
#	 begin			{
		 function						unless(		$subject,			$isReplacedBy									)	# function should sync w/ ~vike/.config/powershell/base.ps1
			{if(									$subject -notMatch	$isReplacedBy[0]){throw}						`
			;return									$subject -replace	$isReplacedBy									}	;
		;																		$Register_ArgumentCompleter=&{			`
			;	(						unless		(get-PSCallStack											`
					)[0].scriptName					-isReplacedBy '(\.[^.]+)$','-Register-ArgumentCompleter$1'		)	}	;
		;&																		$Register_ArgumentCompleter					;
		;if(!$PSBoundParameters.count){"Argument completer registered:",		$Register_ArgumentCompleter}
		
#		;eval(d('$PSBoundParameters','$PSBoundParameters.keys','$PSBoundParameters.values'))|ign
		
		;if	(									$bluelephant)
			{.									$bluelephant}
		;if	(									$bluelephant){write-debug 'setting trap';trap{$null = eval(d_bt('$_'));throw $_;exit 1}}
		
	
		
		# pref
		;$ErrorActionPreference='Stop'
		
#		;if($debugLevel -gt 1){
##			;Set-PSDebug -Trace 1
#			;$PSBoundParameters.Verbose=$true
#			;$VerbosePreference='continue'
#			;}
		
	
		
		# test
#		;return (get-PSCallStack)[0] # .invocationInfo.myCommand.scriptBlock.Ast.findAll({$args[0] -is [Management.Automation.Language.FunctionDefinitionAst]}, $false)|select-object -first 1|%%{param($k);$k}
		
	
		
		# main support
		;																		 $script:keysJsonPath=&{			`
			;	(								unless			(get-PSCallStack							`
					)[0].scriptName								-isReplacedBy '(\.[^.]+)$','-key.json'			)	}	;
		;	 if(								$subShellKeys){					 $script:keys						`
			=									$subShellKeys														;}	`
		 elseif(-not(																		test-path			`
																				 $script:keysJsonPath			)	){
			;																	 $script:keys=@{}					;}	`
		 else{																	 $script:keys						`
			=																		get-content						`
				 																 $script:keysJsonPath				`
			|																	 convertFrom-json -asHash			;}	;
		
		;																		 $script:ignoreKeys=@()
		
		 function																	 get-key							`
			(																			$key						)
			{if	(																		$key					`
				-notIn															 $script:ignoreKeys				)	`
				{													$null -ne	($script:keys.				`
																						$key				)	}	}	;
		 function																	 set-keys							`
			{																	 $script:keys						`
			|																	   convertTo-json					`
			|write-debug																							;
			;																	 $script:keys						`
			|																	   convertTo-json					`
			|																		set-content						`
																				 $script:keysJsonPath				}
		 function																	 set-key							`
			(																			$key,	$value=$true		)
			{if	(																		$key					`
				-notIn															 $script:ignoreKeys				)	`
				{write-debug ((&{	if($null -ne												$value	)`
							{'setting'																	}`
						else{'removing'																	}})	`
					+		' '+														'key'				`
					+':'+	' '+														$key				)	;
				;					if($null -ne												$value		)
					{															 $script:keys.				`
																						$key				`
					=																			$value		}	`
				else{															 $script:keys.				`
					 remove(															$key			)	}	;
				;																 $script:keys					`
				|																   convertTo-json				`
				|write-debug																					;
				;																	 set-keys					}	}	;
		
		# option-like param-wise
		;foreach(																		$key						`
																			in	   $resetKeys						){
			;if(																						$key_force							`
				=																						$key			-match '^force-(.*)')	`
				{																						$key=$matches[					 1]	}	;
			;																		 set-key						`
																						$key	-value $null		}	; # yes, this could - but doesn't need to - be optimized
		;foreach(																		$key						`
																			in		 $setKeys						){
			;if(																						$key_force							`
				=																						$key			-match '^force-(.*)')	`
				{																						$key=$matches[					 1]	}	;
			;																		 set-key						`
																						$key	-value $true		}	; # - ""-
		
		;if	(									$choco_install_force												)	`
			{									$choco_install_args+=							'--force'			}	`
		else{									$choco_install_args												`
			=									$choco_install_args|where-object{$_		-ne		'--force'		}	}	;
		
	
		
		# helper from ~vike/.config/powershell/base.ps1
		 function arrgs		{$args[0..$args.Length]} #cspell:words arrgs
		 function amiAdmin	{	(	[Security.Principal.WindowsPrincipal	]									`
									[Security.Principal.WindowsIdentity		]::	GetCurrent()						`
								).												IsInRole(						`
									[Security.Principal.WindowsBuiltInRole	]::	Administrator					)	}	;
		 function	chkerr	{if(		$LastExitCode){throw 'Native error'	}} #cspell:ignore chkerr
		 function	and		{if(!$? -or	$LastExitCode){throw 'And error'	}}
		
	
		
		# helper
		;											  $psBoundParameters_												`
		=											  $psBoundParameters												;
		 function								 subShellArg{
			;write-debug		(				'subShellArg'													`
				+':'+' '+								$args[0]												)	`
			;			$commandName																				`
			=	(get-PSCallStack																				`
					)[0]. scriptName																				;
#			;switch			(	$PSEdition){
#				'Core'		{	$psInterpreter='pwsh'																							}
#				'Desktop'	{	$psInterpreter='powershell'																					}
#				default		{	$psInterpreter='powershell';write-warning "Using interpreter 'powershell' for unknown `$PSEdition: $PSEdition"	}}
#			;					$psInterpreter=(gcm $psInterpreter).source											;
#			;&					$psInterpreter -noni -nop -c														`
#				{			$c,$b,$a =	$args																		`
#				;echo		$c @b @a																				`
#				;&			$c @b @a }	-arg																			`
#							$commandName	,	$psBoundParameters_													`
#											,-subShellKeys,						 $script:keys						`
#											,	-actions															`
#											,			$args[0]													;
			;																	 $script_keys						`
			=																	 $script:keys						;
			;									start-job															`
				{		   $c,$b,$a	=	$args																	;
				;									  $psBoundParameters_										`
				=								$using:psBoundParameters_										;
#				;. "$HOME\Documents\PowerShell\base.ps1"														`
#				;eval(d(																					`
#					 '$using:commandName'																	`
#					,								 '$psBoundParameters_'									`
#				,														  '$using:script_keys'				))|ign
#				;echo '-- beg'																					`
#				;echo																							`
#					  $using:commandName			  @psBoundParameters_										`
#												  -setKeys @()													`
#												-resetKeys @()													`
#											 -subShellKeys				   $using:script_keys					`
#												-actions:														`
#														$args[0]												`
#				;echo '-- end'																					;
				;&	  $using:commandName			  @psBoundParameters_										`
												  -setKeys @()													`
												-resetKeys @()													`
											 -subShellKeys				   $using:script_keys					`
										   -forceActions:$false													`
												-actions:														`
														$args													;}	`
														-arg														`
														$args														`
			|								  receive-job -wait -autoRemove											;}	# for to contain local function cd (and vars) but this still gets through; recursing cause-of: # > &{function f{'r'};$f=$function:f;$s={&$f};powershell -nop -noni -c $s} # $s doesn't close over $f, $function:f, nor f #cspell:ignore noni
		 set-alias								 sub																	`
												 subShellArg															;
		
		 function								 elevate{
#			;eval(d('$args'))|ign
			;									$args																`
			|&									$elevateBy															}	;
		
	
		
		# dev-test
		 function											dummy
			{									 sub		dummy-req}
		 function											dummy-req{}
		
	
		
		# marker
		 function argumentCompleteBegin{}
		
	
		
		# util
		;																		 $script:ignoreKeys						`
		+=		 'clear-keys'																							;
		 function clear-keys{													 $script:keys=@{}					;
			;																		 set-keys						}	;
		;																		 $script:ignoreKeys						`
		+=		   'get-keys-variable'																					;
		 function	get-keys-variable	{										 $script:keys.keys					}	;
		;																		 $script:ignoreKeys						`
		+=		   'get-keys-content'																					;
		 function	get-keys-content	{get-content							 $script:keysJsonPath				}	;
		
	
		
		# main
		#	~vike bootstrapping
		#	see ./Dockerfile-win
		#	#cspell:ignore modifymedium clonehd keepuuid repart
		# @	host ~vike/.bash_profile
#		# $	cd1 /ı/chrome/automatic/CloneVDI-exe-4.01 wine CloneVDI ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vmdk --output ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vdi --keepuuid --enlarge 160.00GB --repart
#		#	… Source is a difference image. Sorry, this tool cannot clone these (for now)
		# $	VBoxManage clonehd ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vmdk ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vdi --format VDI --variant Standard
		#	… Clone medium created in format 'VDI'. UUID: cd185f88-c564-44ad-9962-8b2e4842e7fc
		# $	VBoxManage modifymedium --resize $((81920*2)) ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vdi
#		#	next makes it "inaccessible" #cspell:ignore showmediuminfo
#		# $	mv ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vdi ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{$(VBoxManage showmediuminfo ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/Snapshots/\{c2c93944-fe85-44b9-9680-4a7fde6fe16a\}.vdi|awk '"UUID:"==$1{print $2}')\}.vdi
		# *	change path\uuid @ VirtualBox GUI or ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/windows-docker-machine_2019-box_1644925935122_93341.vbox
		# @	virtualbox * add storage attachment optical drive & insert VBoxGuestAdditions.iso (VirtualBox.app/Contents/MacOS)
		# @	guest powershell
		# >	D:\VBoxWindowsAdditions.exe
		# @	exe GUI: Next for default "Destination folder" "C:\Program Files\Oracle\VirtualBox Guest Additions", next, "Select components to install" all, esp. "Direct3D Support (Experimental)", install, reboot (might aswell), finish
		# @	host ~vike/.bash_profile
		# $	cd1 /∂/git/windows-docker-machine vagrant halt 2019-box
		# *	display "Enable 3d Acceleration" @ VirtualBox GUI or ~/VirtualBox\ VMs/windows-docker-machine_2019-box_1644925935122_93341/windows-docker-machine_2019-box_1644925935122_93341.vbox getting accelerate3D="true" into <Display controller="VBoxSVGA" VRAMSize="128"/> <!-- oh yeah, I changed the display mode too #cspell:ignore SVGA -->
		# * was must: $	VBoxManage startvm --type gui <uuid|vmname> # $	VBoxManage list vms # for <uuid|vmname> # cf https://github.com/hashicorp/vagrant/issues/2720 #cspell:ignore startvm vmname
		# $	(context=2019-box && export windows_docker_machine_customize=/∂/git/ui/.docker/services/ui/windows-docker-machine.rb && cd /∂/git/windows-docker-machine && VBoxManage startvm --type gui windows-docker-machine_2019-box_1644925935122_93341 && vagrant provision $context)
		# *	note: old instead of previous is okay after updating windows-docker-machine.rb to include `v.gui = true`
		# *	I forgot, I have amd:
		# >	D:\VBoxWindowsAdditions-amd64.exe
		# @	exe GUI as above
		# @	guest powershell
		# >	# >	/usr/src/ui/.docker/services/ui/windows-docker-machine.ps1 run (($HOME -replace 'vagrant','vike') -replace '\\','/') {&'C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe' C:\usr\src\libui\build\meson-out\dateTime.exe}
		# *	NOTE: while previous works, as does replacing timer for dateTime, controlGallery.exe works until next exception activating tab "Data Choosers", still, running the following (w/o/w/o last arg code block, there w/o/w/o replacing tester or drawText for histogram), I get following exception: #cspell:ignore HWND
		#	(83c.740): C++ EH exception - code e06d7363 (first chance)
		#	[libui] c:\usr\src\libui\windows\draw.cpp:87:makeHDCRenderTarget() error creating DC render target: HRESULT == 0x8899000B No hardware rendering device is available for this operation.
		# >	# >	/usr/src/ui/.docker/services/ui/windows-docker-machine.ps1 run (($HOME -replace 'vagrant','vike') -replace '\\','/') {&'C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe' C:\usr\src\libui\build\meson-out\histogram.exe}
		#	(390.3c8): C++ EH exception - code e06d7363 (first chance)
		#	[libui] c:\usr\src\libui\windows\draw.cpp:66:makeHWNDRenderTarget() error creating HWND render target: HRESULT == 0x8899000B No hardware rendering device is available for this operation.
		# *	following readme of https://github.com/StefanScherer/packer-windows
		# *	download ISO and note checksum for packer build from https://www.microsoft.com/en-us/software-download/windows10ISO
		# @	virtualbox * insert ISO
		# >	Get-WindowsImage -ImagePath D:\sources\install.wim|out-file -enc utf8 'C:\usr\src\ui\.docker\services\ui\Get-WindowsImage -ImagePath D__sources_install.wim.txt'
		# *	ignoring variant "N" https://support.microsoft.com/en-us/topic/media-feature-pack-for-n-editions-of-windows-10-version-1607-b657cb70-33e7-1f11-7119-3b4b50be4e89; note: ImageIndex       : 1 & ImageName        : Windows 10 Home
		# *	-or-
		# *	note: ImageIndex       : 2 & ImageName        : Windows 10 Home N
		# @	host ~vike/.bash_profile
		# $	code /∂/git/packer-windows/answer_files/10/Autounattend.xml #cspell:ignore Autounattend unattend
		# *	like webAPI javascript: document.querySelector('unattend>settings>component>ImageInstall>OSImage>InstallFrom>MetaData>Value').textContent='Windows 10 Home'
		# *	-or- (resp. above)
		# *	like webAPI javascript: document.querySelector('unattend>settings>component>ImageInstall>OSImage>InstallFrom>MetaData>Value').textContent='Windows 10 Home N'
		# *	following in-file instructions to rewrite document.querySelector('unattend>settings>component>UserData>ProductKey')
		# $	cd1 /∂/git/packer-windows packer build --only virtualbox-iso --var disk_size=$((81920*2)) --var 'iso_url=/ı/chrome/automatic/Win10_21H2_EnglishInternational_x64.iso' --var 'iso_checksum=06FD4A512C5F3E8D16F77CA909C4F20110329B8CDD5AD101E2AFC0D58B06D416' windows_10.json # brew install packer
		#	…
		#	Build 'virtualbox-iso' finished after 6 hours 26 minutes.
		#	==> Wait completed after 6 hours 26 minutes
		#	==> Builds finished. The artifacts of successful builds are:
		#	--> virtualbox-iso: 'virtualbox' provider box: windows_10_virtualbox.box
		# $	cd1 /∂/git/packer-windows vagrant box add windows_10 windows_10_virtualbox.box
		# @	virtualbox * add storage attachment optical drive & insert VBoxGuestAdditions.iso (VirtualBox.app/Contents/MacOS)
		# @	guest powershell
		# >	D:\VBoxWindowsAdditions-amd64.exe
		# @	exe GUI as above
		
		# docker FAIL begin
		
		# @	host ~vike/.bash_profile
		# $	(context=10 && export windows_docker_machine_customize=/∂/git/ui/.docker/services/ui/windows-docker-machine.rb && cd /∂/git/windows-docker-machine && vagrant up --provider virtualbox $context && vagrant provision $context)
		
		# @	guest powershell -c start -verb runAs powershell # admin privileges elevated any way you like
		 function						  run-packerScripts
			{									cd			$HOME/file/develop/git/packer-windows # obviously dev here
			;													./scripts/docker/set-winrm-automatic.ps1
			;														 $env:docker_images="mcr.microsoft.com/windows/nanoserver:1809 mcr.microsoft.com/windows/servercore:ltsc2019 mcr.microsoft.com/windows/servercore:1809 mcr.microsoft.com/windows:1809" #cspell:ignore nanoserver servercore servercore
			;														 $env:docker_provider="ce"
			;														 $env:docker_version="20.10.7"
			;													./scripts/docker/add-docker-group.ps1
			;													./scripts/wait-for-tiWorker.ps1
			;													./scripts/docker/open-docker-insecure-port.ps1
			;													./scripts/docker/open-docker-swarm-ports.ps1
			;													./scripts/docker/remove-docker-key-json.ps1
			;													./scripts/docker/disable-windows-defender.ps1
			;}
		 function														  docker-cp
			{
			#	#cspell:ignore windowsservercore ltsc
	#		;															  docker cp dotnet-framework-sdk-4_8-windowsservercore-ltsc2019:C:\php-sdk			C:\php-sdk
			;															  docker cp dotnet-framework-sdk-4_8-windowsservercore-ltsc2019:C:\php				C:\php
			;															  docker cp dotnet-framework-sdk-4_8-windowsservercore-ltsc2019:C:\usr\src\pthreads	C:\usr\src
			;															  docker cp dotnet-framework-sdk-4_8-windowsservercore-ltsc2019:C:\usr\local		C:\usr\local
			;}
		 function											choco-install-docker																{
			;									 sub			  install-choco																	;
			# >	choco install  -y --stop-on-first-failure Containers Microsoft-Hyper-V --source windowsfeatures # FAIL # said by: choco info docker-engine #cspell:ignore windowsfeatures
			# >	choco info docker-desktop # said by: choco info docker (being legacy name for docker-cli)
			#	see 'packer-scripts'
			;									 elevate   $choco															@choco_install_args	`
																		  docker-desktop														`
	#																	  docker-machine														`
			;																																	;}
		;																							 $script:ignoreKeys								`
		+=														 'install-docker'																	# only calls sub
		 function												  install-docker																{
			;									 sub		choco-install-docker																;
			;																																	;}	;
		
		# docker FAIL end
		
		# @	host ~vike/.bash_profile
		# $	(context=10 && export windows_docker_machine_customize=/∂/git/ui/.docker/services/ui/windows-docker-machine.rb && cd /∂/git/windows-docker-machine && vagrant up --provider virtualbox $context)
		
		# @	Edge https://aka.ms/terminal installed
		# @	guest powershell
		#	cp "$HOME/AppData/Local/Packages/Microsoft.WindowsTerminal_8wekyb3d8bbwe/LocalState/settings.json" "$(dirname ((Get-PSReadLineOption).HistorySavePath -replace 'PSReadLine','$0_docker_machine'))\settings.json" #cspell:ignore 8wekyb3d8bbwe
		#	cp (Get-PSReadLineOption).HistorySavePath ((Get-PSReadLineOption).HistorySavePath -replace 'PSReadLine','$0_docker_machine')
		
		# @	guest powershell -c start -verb runAs powershell # admin privileges elevated any way you like
		# >	Install-Module -Name PowerShellGet -Force # according to https://github.com/PowerShell/PSReadLine#install-from-powershellgallery-preferred according to https://github.com/PowerShell/PSReadLine#upgrading fixing main issue https://github.com/PowerShell/PSReadLine/issues/1306
		# @ cmd or run after all powershell instances exited # according to https://github.com/PowerShell/PSReadLine#upgrading (next almost)
		#	powershell -noe -nop -c "Install-Module PSReadLine -Force -SkipPublisherCheck -AllowPrerelease"
		
		# @	guest powershell -c start -verb runAs powershell # admin privileges elevated any way you like
		# >	choco install -y powershell-core # for windows-docker-machine-Register-ArgumentCompleter.ps1 - but it didn't help
		
		#	after above I need to run choco with "admin privileges elevated"?
		
		#	installing gSudo I guess always requires "admin privileges elevated"
		# @	guest powershell -c start -verb runAs powershell # admin privileges elevated any way you like
		# >	choco install -y gsudo # I installed "sudo" first but that doesn't support powershell cmdlets #cspell:ignore gSudo
		
		;																							 $script:ignoreKeys								`
		+=														 'install-bins'																		# only calls sub
		 function												  install-bins																	{
			;									 sub			  install-libui																	;
			;									 sub			  install-ext																	;
			;																																	;}	;
		
		 function												  install-choco																	{
			;									mkdir		-force		/machine-workdir											|out-null	;
			;									cd						/machine-workdir														;
			;									Invoke-WebRequest -UseBasicParsing http://community.chocolatey.org/install.ps1 -OutFile chocolatey.ps1
			;&								$elevate																				  .\chocolatey.ps1}
		
		 function											choco-install-optionals																{
			;&									 elevate   $choco															@choco_install_args	`
												gsudo																							`
												sed																								`
												vim																								`
												less																							;}	#cspell:ignore gSudo
		
		 function												  install-libui																	{
			;									 sub		choco-install-buildtools-lib
			;									cd					  /usr/src/libui
			;									mkdir		-force	C:\usr\local													|out-null	;
			;write-warning "To mitigate ninja getting ""Clock skew detected"", we'll try to force resynchronizing the clock over the network time protocol. I still have to retry every once in a while. A message that ""The service has not been started. (0x80070426)"" might be mitigated by running ``start-service w32time``."
			;&									 elevate {w32tm /resync /force}
			;&									 elevate {	pwd|write-verbose} # just clarifying cwd (in elevated ctx) wh/as meson has sub-optimal error message/behavior with bad paths
			;&									 elevate {	cmd /c			 ('"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64	&&		meson setup --wipe	build																		')}	;chkerr # w/o barrage of apostrophe-backtick-linebreak(-whitespace)-plus-apostrophe meson got a literal backtick and w/o parens it got space-plus
			;&									 elevate {	cmd /c			 ('"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64	&&		ninja -v -C			build																		')}	;chkerr
			;&									 elevate {	cmd /c			 ('"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64	&&		ninja -v -C			build install																')}	;chkerr
			;&									 elevate {	cmd /c			 ('"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64	&&		ninja -v -C			build -t	compdb															'`
				+'																																													>				build/compile_commands.json													')}	;chkerr # - "" -
			#																																													#cspell:ignore					compdb																#
			;																																	;}	;
		
		 function											choco-install-buildtools-lib														{
			;									 sub		choco-install-buildtools-vs															;
			;&									 elevate   $choco															@choco_install_args	`
												ninja																							`
												meson																							;
			;$env:PATH+=${ENV:ProgramFiles} + '\Meson'																						+';';}	;
		 function											choco-install-buildtools-vs															{
			;									 sub			  install-choco																	;
#			;if(-not(amiAdmin)){throw "action requires admin privileges elevated: "
#														 + "choco-install-buildtools-vs"}
			# >	C:\ProgramData\chocolatey\lib\dependencies\tools\Dependencies -depth 1 -chain C:\php\debug\php.exe
			#	#cspell:ignore redist vcredist
			# C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Redist\MSVC\
#			#	next doesn't help
#			# >	choco install vcredist2017
			#	#cspell:ignore choco buildtools vctools
			;write-warning "The following failed, seemingly random, many times, and I did reboot one time in between, until visualstudio2017-workload-vctools got installed"
			;&									 elevate   $choco															@choco_install_args	`
												visualstudio2017buildtools																		`
												visualstudio2017-workload-vctools																`
												--package-params '--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64'						;
##												nuGet.commandLine																				` # i think this was for the path of sharpDX
#			;	Invoke-WebRequest -UseBasicParsing https://download.microsoft.com/download/8/4/A/84A35BF1-DAFE-4AE8-82AF-AD2AE20B6B14/directx_Jun2010_redist.exe	`
#				-OutFile																											  directx_Jun2010_redist.exe	; #cspell:ignore directx
##			;	Invoke-WebRequest -UseBasicParsing https://download.microsoft.com/download/1/7/1/1718CCC4-6315-4D8E-9543-8E28A4E18C4C/dxwebsetup.exe				`
##				-OutFile																											  dxwebsetup.exe				;
##			;																														.\dxwebsetup.exe				; #cspell:ignore dxwebsetup
			# >	/usr/src/ui/.docker/services/ui/windows-docker-machine.ps1 run (($HOME -replace 'vagrant','vike') -replace '\\','/')
			;																																	;}	;
		 function											choco-install-buildtools-ext-reqs													{
			;									 sub		choco-install-buildtools-vs															;
			;&									 elevate   $choco															@choco_install_args	`
												git																								`
												gawk																							`
												patch																							`
			;$env:PATH+=${ENV:ProgramFiles} + '\Git\cmd'																					+';';}	;
		 function												  install-buildtools-ext-req-pthreads											{
			#cspell:ignore pthreads pthread																										#
#			;C:\ProgramData\chocolatey\lib\Wget\tools\wget -P	C:\usr\local\pthreads-latest -nH --cut-dirs=3									`
#				-r									ftp://sourceware.org/pub/pthreads-win32/dll-latest/											`
			# Building cause-of "MSVCR100.dll (NOT_FOUND)" in dependencies.exe run (below)														#cspell:ignore MSVCR
			;									mkdir		-force		/machine-workdir											|out-null	;
			;									cd						/machine-workdir														;
			;									Invoke-WebRequest -UseBasicParsing																`
											http://www.mirrorservice.org/sites/sourceware.org/pub/pthreads-win32/pthreads-w32-2-9-1-release.zip	`
											-OutFile															 pthreads-w32-2-9-1-release.zip	`
			;									Expand-Archive													 pthreads-w32-2-9-1-release.zip	`
				-DestinationPath																												`
													/usr/src/pthreads																	-force	`
			;									cd	/usr/src/pthreads/pthreads.2																`
			# patching cause-of "pthread.h(320): error C2011: 'timespec': 'struct' type redefinition", fixed in ui.c context #define HAVE_STRUCT_TIMESPEC, but there are issues adding /DHAVE_STRUCT_TIMESPEC: to Nmakefile has no effect on "cl" command, to Makefile has that effect but doesn't mitigate error ... so i'm giving up and hack with a patch #cspell:ignore dhave timespec
			;									cp	pthread.h																					`
													pthread.h.orig																				`
			;																																	;
			;										$PSScriptBase																				`
			=	(							unless (get-PSCallStack																		`
					)[								0].scriptName -isReplacedBy '(\.[^.]+)$',''												)	;
			;									cp	$PSScriptBase'.d'/pthread.h.patch															`
												,	$PSScriptBase'.d'/pthread.h.orig															`
											 -dest	/usr/src/pthreads/pthreads.2/																;
			;									cd	/usr/src/pthreads/pthreads.2																`
			;								 patch	pthread.h																					`
													pthread.h.patch																				`
			# C:\usr\src\pthreads\pthreads.2\readme says, in length, apart from V being visual, there are C++, "[S]tructured" and "C cleanup" exception handling modes w/ or, as I chose, w/o extra acronym suffix E; continuing there's a list of these acronyms combined with, one instance of "static", but many of (there unexplained) inline, and debug, where the latter could be of interest but probably not, plus I'm unsure if it could matter that ui/config.w32 specifies "pthreadVC2.lib" which isn't debug #cspell:ignore tructured nmake nmakefile
			# Also, I wouldn't mind install-prefixing this with-in /usr/local, but there's no config*, and I don't care enough to look into that mess of makefiles (and minimalist Nmakefile) and chains... #cspell:ignore makefiles
			;												cmd /c			 ('"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64	&& nmake clean VC')			;chkerr	;} # SHOULD install by cp or prefix, templated by /usr/src/pthreads/Pre-built.2, under /usr/local/pthreads: pthread.h, sched.h, and semaphore.h under include; pthreadGC2.dll, and pthreadVC2.dll under dll/x64; and pthreadVC2.lib under lib/x64 #cspell:ignore sched
		 function											clone-ext-buildtools-php-sdk																																	{
			;									git			clone https://github.com/Microsoft/php-sdk-binary-tools.git																	c:\php-sdk					;chkerr	}
		;																							 $script:ignoreKeys							`
		+=									   'cd-php-base'																					;
		 function								cd-php-base{ # c:\php-sdk\phpmaster\vc15\x64\php-src											#
			;									cd																																		c:\php-sdk	`
			;									cd																															phpmaster				`
			;									cd									 vc15														`
			;									cd											 x64												;}	;
		 function											clone-ext-buildtools-php-src														{
			;									cd-php-base																						;
			;									git			clone https://github.com/php/php-src.git									;chkerr	}
		;																							 $script:ignoreKeys							`
		+=									   'cd-php-src'																						;
		 function								cd-php-src{ # c:\php-sdk\phpmaster\vc15\x64\php-src												#
			;									cd-php-base																						;
			;									cd									 php-src													;}	;
		;																							 $script:ignoreKeys							`
		+=												  'php-sdk-command'																		;
		 function										   php-sdk-command																		{
#			;echo										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args ('-noni -nop -c '+(&{if($args){$args -join ' '}else{$command}} @args))
			;											c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args ('-noni -nop -c '+(&{if($args){$args -join ' '}else{$command}} @args))
			;																															;chkerr	;}	;
		 function												  install-ext																	{
			;									 sub		choco-install-buildtools-ext-reqs													`
			;									 sub			  install-buildtools-ext-req-pthreads											`
			;									 sub		clone-ext-buildtools-php-sdk														# sub cause-of can't clone onto existing
			;									cd																																		c:\php-sdk	`
			# sort is powershell sort-object; we could pass -descending but that would break current *nix compat in sh-like command subst (albeit broken by win escaping)
			;									git checkout																																		`
			$(									git tag -l|sort|gawk 'match($0,\"^php-sdk-2\\.2(\\.[0-9.]+)?$\",m){h=m[0]}END{print h}')															`
			#cspell:ignore																												  noni	#
			#									visualstudio2019					 vs16 for "master" (re below)								#
			#									visualstudio2017					 vc15 for php7.2-7.4										#
			#									visualstudio2015					 vc14 for php7.0-7.1										#
			#cspell:ignore																																phpsdk buildtree	phpmaster
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c	phpsdk_buildtree	phpmaster	c:\php-sdk'`
			;and;										   php-sdk-command																				phpsdk_buildtree	phpmaster	c:\php-sdk
			;and;								cd-php-base																						;
			;and;								 sub		clone-ext-buildtools-php-src														# sub cause-of can't clone onto existing
#			;									Invoke-WebRequest -UseBasicParsing																`
#								 https://windows.php.net/downloads/releases/php-7.4.27-src.zip													`
##				-OutFile													php-7.4.27-src.zip													`
##			;	Expand-Archive . -path										php-7.4.27-src.zip													`
#				-OutFile														 php-src.zip													`
#			;	Expand-Archive . -path											 php-src.zip													`
			;and;								cd								 php-src														`
			;and;								git checkout																					`
			$(									git tag -l|sort|gawk 'match($0,\"^php-7\\.4(\\.[0-9.]+)?$\",m){h=m[0]}END{print h}')			`
			#Invoke-WebRequest -UseBasicParsing https://windows.php.net/downloads/releases/php-7.4.27-src.zip -OutFile php-7.4.27-src.zip;Expand-Archive . -path php-7.4.27-src.zip
			#cspell:ignore																																phpsdk deps
##			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c	phpsdk_deps --update --branch '`
			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c	phpsdk_deps --update --branch '`
			+(									git branch --remote|sort|gawk 'match($0,\"^\\s*origin/PHP-(7\\.4(\\.[0-9.]+)?)$\",m){h=m[1]}END{print h}')
			# what the f*ck is up with the diff in result between around commands!? i'm giving up, so NOTE the fuc*ing lack of use of php-sdk-command in prev
#			;and;										   php-sdk-command																			  ('phpsdk_deps --update --branch '`
#			+(									git branch --remote|sort|gawk 'match($0,\"^\\s*origin/PHP-(7\\.4(\\.[0-9.]+)?)$\",m){h=m[1]}END{print h}'))
##			# c:\php-sdk/doc/unattended-build-task.bat.example uses --enable-snapshot-build, and buildconf says to check configure --help, where I note the latter on the former, which explains "turns on everything it can and ignores build errors"
##			# The diff in the following configure --help, though, I'm not sure, but I'm thinking, it might be related to https://github.com/docker-library/php/issues/879, but that might be just a wild assoc
##			#	--enable-debug       Compile with debugging symbols
##			#	--enable-debug-pack  Release binaries with external debug symbols (--enable-debug must not be specified)
			#cspell:ignore																																  buildconf
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args ('-noni -nop -c .\buildconf --add-modules-dir=C:\usr\src')`
			;and;										   php-sdk-command																				.\buildconf --add-modules-dir=C:\usr\src
			# didn't notice at first: WARNING: Debug builds cannot be built using multi processing (independent of --with-ui)
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-all --enable-debug --enable-cli --with-ui='`
			#cd c:\php-sdk\phpmaster\vc15\x64\php-src;c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\buildconf --add-modules-dir=C:\usr\src';c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-all --enable-debug --enable-cli --with-ui=''C:\usr\local\libui\include;C:\usr\local\libui\lib;C:\usr\local\pthreads-latest\include;C:\usr\local\pthreads-latest\lib\x64,shared'''
##			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-all --enable-cli --with-ui='`
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args ('-noni -nop -c .\configure --disable-cgi --enable-debug --with-ui='`
			;and;										   php-sdk-command																				.\configure --disable-cgi --enable-debug --enable-mbstring --with-ui=$(''`
			+															'''' + 'C:\include'																											`
			+															 ';' + 'C:\lib'																												`
			+															 ';' + 'C:\usr\src\pthreads\pthreads.2'																						`
			+															 ',' + 'shared'																												`
			+															''''																														)	`
			|									tee																										.\configure.log									# someone does cls after possibly vital output from ./configure #cspell:ignore mbstring
			# dependencies -depth 2 -chain C:\usr\local\libui\lib\libui.lib|select-string 'NOT_FOUND' -Context 666 # happens to match "ext-ms-win32-subsystem-query-l1-1-0.dll" under "ole32.dll" under root "libui.dll"
			# DEBUG:    1+ Set-PSDebug -Trace 1; >>>> .\configure --disable-cgi --enable-debug --with-ui=
			# Checking for ui.h ...  <not found> ¬ WARNING: libui not found; libraries and headers not found
			# prev comes from next in php-src/win32/build/confutils.js #cspell:ignore confutils
			# CHECK_HEADER_ADD_INCLUDE("ui.h", "CFLAGS_UI", PHP_UI + ";" + configure_module_dirname) #cspell:ignore CFLAGS
###			+														 ';' + 'C:\usr\local\libui\bin'																									`
##			+														 ';' + 'C:\usr\local\pthreads-latest\include'																					`
##			+														 ';' + 'C:\usr\local\pthreads-latest\lib\x64'																					`
###			+														 ';' + 'C:\usr\local\pthreads-latest\dll\x64'																					`
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-cgi --disable-zts --enable-debug --with-ui='`
			#cd c:\php-sdk\phpmaster\vc15\x64\php-src;c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\buildconf --add-modules-dir=C:\usr\src';c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-cgi --enable-debug --with-ui=''C:\usr\local\libui\include;C:\usr\local\libui\lib;C:\usr\local\pthreads-latest\include;C:\usr\local\pthreads-latest\lib\x64,shared'''
			#cd c:\php-sdk\phpmaster\vc15\x64\php-src;c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\buildconf --add-modules-dir=C:\usr\src';c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-cgi --enable-debug --with-ui=''C:\usr\local\libui\include;C:\usr\local\libui\bin;C:\usr\local\pthreads-latest\include;C:\usr\local\pthreads-latest\dll\x64,shared'''
			#cd c:\php-sdk\phpmaster\vc15\x64\php-src;c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\buildconf --add-modules-dir=C:\usr\src';c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-cgi --enable-debug --with-ui=''C:\usr\local\libui\include;C:\usr\local\libui\lib;C:\usr\src\pthreads,shared'''
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c .\configure --disable-cgi --with-ui='`
			#w/ --enable-debug link (via nmake) gets many errors mentioning things like crc32, deflate, inflate, gz, some libiconv, quite a few html, a lot xml{Text{Reader,Writer},XPath,Reader} (but not Writer and not much on xmlFile) #cspell:ignore simplexml xmlreader xmlwriter
			#--without-libxml                  LibXML support
			#--without-dom                     DOM support
			#--without-simplexml               Simple XML support
			#--without-xml                     XML support
			#--disable-xmlreader               XMLReader support
			#--with-xmlrpc                     XMLRPC-EPI support
			#--disable-xmlwriter               XMLWriter support
			#--with-xsl                        xsl support
##			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args '-noni -nop -c $env:LDFLAGS=''/LIBPATH:C:\usr\local\libui\lib'';$env:CFLAGS_UI=''/IC:\usr\local\libui\include'';.\configure --disable-all --enable-cli --with-ui=C:\usr\local\libui #cspell:ignore LDFLAGS LIBPATH
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args ('-noni -nop -c nmake'										)	`
			;and;										   php-sdk-command																				nmake											`
#			;and;										c:\php-sdk\phpsdk-starter -c vc15 -a x64 -t (gcm powershell).source --task-args ('-noni -nop -c nmake install'								)	`
			;and;										   php-sdk-command																				nmake install									`
			;chkerr																																;

			;																																	;}
		 function												  install-run-reqs																{
			;									 sub			  install-choco																	;
			;&									 elevate   $choco															@choco_install_args	`
												dependencies																					;
			;&									 elevate   $choco															@choco_install_args	`
												windows-sdk-10-version-1809-windbg																; # vagrant virtualbox box 2019-box is windows server 2019 which, cf windows-versions.rtf, means YYMM version 1810 (between 1803 and actually 1809 - latter actually released 1811) #cspell:ignore yymm #cspell:words windbg
			;}
		;																							 $script:ignoreKeys							`
		+=														 'run'																			;
		 function												  run																			{
			;									 sub			  install-bins																	;
			;									 sub			  install-run-reqs																;
			;$var										=		'PATH'																			;
#																			'C:\usr\local\pthread\bin'											`
			;foreach(							$path in		arrgs		'C:\php\debug'														`
																			'C:\usr\src\pthreads\pthreads.2'									`
																			'C:\bin'															){	[Environment]::SetEnvironmentVariable($var
					,																																[Environment]::GetEnvironmentVariable($var)+(get-item	$path).fullName+';','Process')}
			;									dependencies -depth 2	-chain																	`
																			'C:\php\debug\ext\php_ui.dll'|select-string 'NOT_FOUND' -Context 666;
#			;php -n -d extension_dir=C:\php\debug\ext -d extension=C:\php\debug\ext\php_ui.dll -r '\UI\quit();'
			;write-debug 'beg runCode'
			;&																$runCode															`
																			$runBase															;
			;write-debug 'end runCode'
			;																																	;}
		
	
		
		# marker
		 function argumentCompleteEnd{}
		
	
		
		# test
#		;write-error "`$?:$?;`$LASTEXITCODE:$LASTEXITCODE" # (-not $? -or $LASTEXITCODE) #cspell:ignore LASTEXITCODE
		
	
		
		# main
		;foreach(																						$action									`
																								in		$actions								){
#			;if(																					$skipActions							`
#				-and																				$skipActions--							){
#				;																				continue									;}	;
			;if(																						$action_force						`
				=																						$action			-match '^force-(.*)')	`
				{																						$action=$matches[				 1]	}	;
			;																		$set														`
			=		-not																					   $forceActions					`
			-and	-not																				   $deepForceActions					`
			-and	-not																				$action_force							`
			-and  (																	 get-key			$action								)	;
			;	if(																	$set													`
				-and																$null		-eq		$subShellKeys						)	# this is common "error" and though `throw` or `write-error` works fine w/ current subShell setup, it's a long message by default, plus I use and even more verbose trap block
				{write-host (												"switch -forceActions:`$true, -deepForceActions:`$true, set -resetKey[s]:key[,key[…]], pass action clear-keys, or pass action prefixed with force-, for already set key: "`
					+																					$action							)	;}	`
			elseif(															-not	$set													){
				;write-verbose (																		'action'						`
					+':'+' '+																			$action							)	`
				;																			&			$action								; # CHECK TODO SHOULD use sub? though then eg cd-php-src wouldn't work and would need exemption?
				;																	 set-key			$action								;}	;}	;
		
	
		
#		} # begin
