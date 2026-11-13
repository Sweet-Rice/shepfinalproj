[CmdletBinding()]
param(
    [ValidateSet('Build', 'Test', 'Play', 'Package')]
    [string]$Mode = 'Test',
    [string]$EngineRoot = (Join-Path $env:ProgramFiles 'Epic Games\UE_5.8'),
    [string]$AssetsFrom,
    [string]$PackageDirectory
)

$ErrorActionPreference = 'Stop'
$projectRoot = Join-Path $PSScriptRoot 'PFTFloor1Viewer'
$project = Join-Path $projectRoot 'PFTFloor1Viewer.uproject'
$build = Join-Path $EngineRoot 'Engine\Build\BatchFiles\Build.bat'
if (-not (Test-Path -LiteralPath $build -PathType Leaf)) {
    throw "Unreal build tools not found at $build. Set -EngineRoot to your Unreal 5.8 installation."
}

if ($AssetsFrom) {
    $source = (Resolve-Path -LiteralPath $AssetsFrom).Path
    foreach ($relative in 'Content\Maps\PFTWalkthrough.umap', 'Plugins\BasicMasterMaterials\BasicMasterMaterials.uplugin', 'Plugins\DTGenericCatalog\DTGenericCatalog.uplugin') {
        if (-not (Test-Path -LiteralPath (Join-Path $source $relative) -PathType Leaf)) {
            throw "Missing $relative under $source. Use the curated PFTFloor1Viewer walkthrough."
        }
    }
    foreach ($folder in 'Content', 'Plugins') {
        $destination = Join-Path $projectRoot $folder
        if (-not (Test-Path -LiteralPath $destination)) {
            New-Item -ItemType Junction -Path $destination -Target (Join-Path $source $folder) | Out-Null
        }
    }
}

if ($Mode -ne 'Build') {
    foreach ($relative in 'Content\Maps\PFTWalkthrough.umap', 'Plugins\BasicMasterMaterials\BasicMasterMaterials.uplugin', 'Plugins\DTGenericCatalog\DTGenericCatalog.uplugin') {
        if (-not (Test-Path -LiteralPath (Join-Path $projectRoot $relative) -PathType Leaf)) {
            throw "Missing $relative. Pass -AssetsFrom with the curated walkthrough project directory."
        }
    }
}

if ($Mode -eq 'Package') {
    if (-not $PackageDirectory) {
        $PackageDirectory = Join-Path $projectRoot 'Saved\Standalone'
    }
    $PackageDirectory = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($PackageDirectory)
    $uat = Join-Path $EngineRoot 'Engine\Build\BatchFiles\RunUAT.bat'
    & $uat BuildCookRun "-project=$project" -noP4 -platform=Win64 -clientconfig=Development `
        -build -cook -stage -package -pak -iostore -compressed -prereqs -nodebuginfo -unattended -utf8output `
        '-map=/Game/Maps/PFTWalkthrough' "-stagingdirectory=$PackageDirectory" '-ubtargs=-MaxParallelActions=1 -NoUBA'
    exit $LASTEXITCODE
}

& $build PFTFloor1ViewerEditor Win64 Development "-Project=$project" -WaitMutex -MaxParallelActions=1 -NoUBA
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if ($Mode -eq 'Build') { exit 0 }

$map = '/Game/Maps/PFTWalkthrough?game=/Script/PFTFloor1Viewer.PFTSunRunMode'
$editorDir = Join-Path $EngineRoot 'Engine\Binaries\Win64'
$common = @('-game', '-nosplash', '-NoLiveCoding', '-asynctexturecompilationmaxconcurrency=1', '-asyncstaticmeshcompilationmaxconcurrency=2')
if ($Mode -eq 'Play') {
    Start-Process -FilePath (Join-Path $editorDir 'UnrealEditor.exe') -WorkingDirectory $projectRoot -ArgumentList (@(
        ('"{0}"' -f $project), $map, '-dx11', '-windowed', '-ResX=1280', '-ResY=720'
    ) + $common)
    return
}

$logDir = Join-Path $projectRoot 'Saved\Logs'
New-Item -ItemType Directory -Path $logDir -Force | Out-Null
$log = Join-Path $logDir ("SunRun-{0:yyyyMMdd-HHmmss}-{1}.log" -f (Get-Date), $PID)
& (Join-Path $editorDir 'UnrealEditor-Cmd.exe') $project $map @common -nullrhi -NoShaderCompile -PFTVerifySunRun -unattended -nosound -NoCrashDialog "-abslog=$log"
$runExit = $LASTEXITCODE
Write-Host "Unreal test log at $log"
if (Test-Path -LiteralPath $log -PathType Leaf) {
    Select-String -LiteralPath $log -SimpleMatch 'PFT SUN RUN:' | ForEach-Object { $_.Line }
}
if ($runExit -ne 0) { exit $runExit }
if (-not (Test-Path -LiteralPath $log -PathType Leaf) -or
    -not (Select-String -LiteralPath $log -SimpleMatch 'PFT SUN RUN: PASS all walking routes' -Quiet)) {
    throw 'Unreal exited without completing the Sun Run checks.'
}
