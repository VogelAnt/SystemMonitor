param (
    [switch]$force = $false,
    [bool]$gtest = $false,
    [bool]$nlohmann = $true,
    [bool]$open62541 = $true,
    [bool]$redistorium = $false
)

7z *>$null; if (!$?) {
    Write-Output "The directory for 7z.exe is not in PATH."; exit 1
}

if ($force) {
    Write-Output "Removing existing external libraries folder..."
    Remove-Item external -Recurse -Force
}

# create external directory if it's not already there
if ([IO.Directory]::Exists((Join-Path(Get-Location) 'external'))) {
    Write-Output "Working with existing external libraries folder. To force setup with new folder, use the -force switch.";
}
else {
    mkdir external
}

Set-Location external
Write-Output "Downloading external libraries..."







if ($gtest) {
    $googletest_archive = "googletest.zip"
    $googletest_download_target = "$PWD/$googletest_archive"
    $googletest_url = "https://github.com/google/googletest/archive/master.zip"

    if (Test-Path googletest) {
        Remove-Item googletest -Recurse -Force
    }
    if (Test-Path $$googletest_archive) {
        Remove-Item $$googletest_archive
    }
    Write-Output "Downloading googletest..."
    (New-Object System.Net.WebClient).DownloadFile($googletest_url, $googletest_download_target); if (!$?) {
        Write-Output "Could not download googletest."; exit 1
    }
    7z x $googletest_download_target; if (!$?) {
        Write-Output "Could not extract $googletest_archive."; exit 1
    }
    Remove-Item $googletest_archive
    Rename-Item googletest-master googletest
}



if ($nlohmann) {
    if (Test-Path json) {
        Set-Location json
        Write-Output "Updating nlohmann json..."
        git pull; if (!$?) {
            Write-Output "Could not update nlohmann json."; exit 1
        }
        Set-Location ..
    }
    else {
        Write-Output "Downloading nlohmann json..."
        git clone https://github.com/nlohmann/json.git; if (!$?) {
            Write-Output "Could not download nlohmann json."; exit 1
        }
    }
}



if ($open62541) {
    if (Test-Path open62541) {
        Set-Location open62541
        Write-Output "Updating open62541..."
        git pull; if (!$?) {
            Write-Output "Could not update open62541."; exit 1
        }
        Set-Location ..
    }
    else {
        Write-Output "Downloading open62541..."
        git clone https://github.com/open62541/open62541.git; if (!$?) {
            Write-Output "Could not download open62541."; exit 1
        }
    }
    Set-Location open62541
    if (![IO.Directory]::Exists((Join-Path(Get-Location) 'build'))) {
        mkdir build
    }
    Set-Location build
    cmake -DBUILD_SHARED_LIBS=ON -DUA_ENABLE_AMALGAMATION=ON ..
    MSBuild .\open62541.sln /m /p:Configuration=Release
    Set-Location ../..
}



if ($redistorium) {
    if (Test-Path redistorium) {
        Set-Location redistorium
        Write-Output "Updating redistorium..."
        git pull; if (!$?) {
            Write-Output "Could not update redistorium."; exit 1
        }
        Set-Location ..
    }
    else {
        Write-Output "Downloading redistorium..."
        git clone https://gitex.itq.de/smart4i_v3/libraries/redistorium.git; if (!$?) {
            Write-Output "Could not download redistorium."; exit 1
        }
    }
    Set-Location redistorium
    ./ci-scripts/build.ps1
    Set-Location ..
}



Set-Location ..