#include "Modules/ModuleManager.h"
#include "MoviePlayer.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

class FPFTFloor1ViewerModule : public FDefaultGameModuleImpl
{
public:
    void StartupModule() override
    {
        if (IGameMoviePlayer* MoviePlayer = IsRunningGame() ? GetMoviePlayer() : nullptr)
        {
            MoviePlayer->OnPrepareLoadingScreen().AddRaw(this, &FPFTFloor1ViewerModule::PrepareLoadingScreen);
        }
    }

    void ShutdownModule() override
    {
        if (IGameMoviePlayer* MoviePlayer = GetMoviePlayer())
        {
            MoviePlayer->OnPrepareLoadingScreen().RemoveAll(this);
        }
    }

private:
    void PrepareLoadingScreen()
    {
        FLoadingScreenAttributes Screen;
        Screen.WidgetLoadingScreen =
            SNew(SVerticalBox)
            + SVerticalBox::Slot().VAlign(VAlign_Center).HAlign(HAlign_Center)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(NSLOCTEXT("PFTSunRun", "Title", "PFT Sun Run"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 28))
                ]
                + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 10, 0, 16)
                [
                    SNew(STextBlock)
                    .Text(NSLOCTEXT("PFTSunRun", "LoadingMap", "Loading Patrick F. Taylor Hall..."))
                ]
                + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
                [
                    SNew(SThrobber)
                ]
            ];
        GetMoviePlayer()->SetupLoadingScreen(Screen);
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FPFTFloor1ViewerModule, PFTFloor1Viewer, "PFTFloor1Viewer");
