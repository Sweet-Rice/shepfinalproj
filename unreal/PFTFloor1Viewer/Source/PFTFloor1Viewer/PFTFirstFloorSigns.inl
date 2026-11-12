#include "Components/TextRenderComponent.h"
#include "Engine/Font.h"

namespace
{
    enum class EWayfindingFont : uint8 { Board, Plaque, Arrow };

    struct FWayfindingText
    {
        const TCHAR* Text;
        EWayfindingFont Font;
        FVector Location;
        FRotator Rotation;
        FVector Scale;
        float Height;
        float XScale;
        float YScale;
        float HorizontalSpacing;
        float VerticalSpacing;
        FColor Color;
    };

    const FWayfindingText FirstFloorSignText[] = {
#include "PFTFirstFloorSignText.inl"
    };

    int32 RestoreFirstFloorSigns(AActor& Owner)
    {
        const TCHAR* FontPaths[] = {
            TEXT("/Game/Fonts/FFMetaGeorgian.FFMetaGeorgian"),
            TEXT("/Game/Fonts/AcuminProExtraCondensedBold.AcuminProExtraCondensedBold"),
            TEXT("/Game/Fonts/Wingdings.Wingdings"),
        };
        UFont* Fonts[UE_ARRAY_COUNT(FontPaths)]{};
        for (int32 Index = 0; Index < UE_ARRAY_COUNT(FontPaths); ++Index)
        {
            Fonts[Index] = LoadObject<UFont>(nullptr, FontPaths[Index], nullptr, LOAD_NoWarn);
        }

        int32 RegisteredCount = 0;
        for (const FWayfindingText& Sign : FirstFloorSignText)
        {
            UTextRenderComponent* Lettering = NewObject<UTextRenderComponent>(&Owner);
            if (!Lettering->Font || !Lettering->TextMaterial)
            {
                continue;
            }

            UFont* Font = Fonts[static_cast<uint8>(Sign.Font)];
            if (Font)
            {
                Lettering->SetFont(Font);
            }

            // Preserve the glyph direction before applying the authored rotation.
            const TCHAR* Text = !Font && Sign.Font == EWayfindingFont::Arrow ? TEXT("^") : Sign.Text;
            Lettering->SetText(FText::AsCultureInvariant(Text));
            // Match the compact faces on the curated board meshes.
            float Fit = 1.f;
            if (Sign.Font == EWayfindingFont::Board)
            {
                const bool bMultiline = FCString::Strchr(Sign.Text, TCHAR('\n')) != nullptr;
                Fit = Font && !bMultiline ? .75f : .60f;
            }
            Lettering->SetWorldSize(Sign.Height * Fit);
            Lettering->SetXScale(Sign.XScale);
            Lettering->SetYScale(Sign.YScale);
            Lettering->SetHorizSpacingAdjust(Sign.HorizontalSpacing * Fit);
            Lettering->SetVertSpacingAdjust(Sign.VerticalSpacing * Fit);
            Lettering->SetHorizontalAlignment(EHTA_Left);
            Lettering->SetVerticalAlignment(EVRTA_TextBottom);
            Lettering->SetTextRenderColor(Sign.Color);
            Lettering->SetCastShadow(false);
            // Keep glyphs just in front of the sign face to avoid depth flicker.
            const FVector FaceOffset = 1.5 * Sign.Rotation.Vector();
            Lettering->SetWorldTransform(FTransform(Sign.Rotation, Sign.Location + FaceOffset, Sign.Scale));
            Owner.AddInstanceComponent(Lettering);
            Lettering->RegisterComponent();
            RegisteredCount += Lettering->IsRegistered() && Lettering->IsVisible();
        }
        return RegisteredCount;
    }
}
