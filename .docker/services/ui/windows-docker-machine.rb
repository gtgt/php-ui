def customize(config)
  home = ENV['HOME'].gsub('\\', '/')
  {
    home+"/.config/powershell"                                =>  "/Users/vagrant/Documents/WindowsPowerShell",
    home+"/.config/pwsh"                                      =>  "/Users/vagrant/Documents/PowerShell", # host is `ln -s powershell ~/.config/pwsh` # for guest `choco install powershell-core` # aka pwsh v7 #cspell:words choco pwsh
    home+"/.local/share/powershell/PSReadLine_docker_machine" =>  "/Users/vagrant/AppData/Roaming/Microsoft/Windows/PowerShell/PSReadLine_docker_machine",
    home+"/file/develop/git/libui"                            =>  "/usr/src/libui",
    home+"/file/develop/git/ui"                               =>  "/usr/src/ui",
    "/opt/local/etc/php74"                                    =>  "/opt/local/etc/php74",
  }.each {|key, val|
    config.vm.synced_folder key, val
  }

  config.vm.define "10", autostart: false do |cfg|
    cfg.vm.box     = "windows_10"
#   cfg.vm.provision "shell", path: "scripts/create-machine.ps1", args: "-machineHome #{home} -machineName 10"
    customize_cfg config, "10", cfg if defined? customize_cfg
  end

  config.vm.provider "virtualbox" do |v, override|
    v.gui = true
    v.customize ["modifyvm",      :id, "--vram",                  "256"     ] #cspell:ignore modifyvm
    v.customize ['setextradata',  :id, 'GUI/LastGuestSizeHint',   '1024,768'] #cspell:ignore setextradata
    v.customize ['setextradata',  :id, 'GUI/MaxGuestResolution',  'any'     ] #cspell:ignore setextradata
  end
end