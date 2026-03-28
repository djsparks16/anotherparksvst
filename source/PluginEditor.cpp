#include "PluginEditor.h"

namespace
{
    using namespace juce;

    Colour bg0()          { return Colour(0xff04060d); }
    Colour bg1()          { return Colour(0xff08111f); }
    Colour bg2()          { return Colour(0xff0c1730); }
    Colour panel0()       { return Colour(0xff08101c); }
    Colour panel1()       { return Colour(0xff0d1627); }
    Colour panel2()       { return Colour(0xff111d31); }
    Colour textHi()       { return Colours::white.withAlpha(0.95f); }
    Colour textMed()      { return Colours::white.withAlpha(0.72f); }
    Colour textLo()       { return Colours::white.withAlpha(0.50f); }
    Colour displayA()     { return Colour(0xff39e5ff); }
    Colour displayB()     { return Colour(0xffae7bff); }
    Colour oscACol()      { return Colour(0xff40e4ff); }
    Colour oscBCol()      { return Colour(0xffff64c6); }
    Colour subCol()       { return Colour(0xff58f3c1); }
    Colour filterCol()    { return Colour(0xffc2ff4f); }
    Colour bassCol()      { return Colour(0xffff9858); }
    Colour envCol()       { return Colour(0xff58c7ff); }
    Colour lfoCol()       { return Colour(0xff78ffb4); }
    Colour fxCol()        { return Colour(0xffff74a8); }
    Colour perfCol()      { return Colour(0xffa48cff); }
    Colour modCol()       { return Colour(0xff6ea2ff); }

    StringArray octaveItems()  { return { "-4","-3","-2","-1","0","1","2","3","4" }; }
    StringArray semiItems()    { return { "-12","-11","-10","-9","-8","-7","-6","-5","-4","-3","-2","-1","0","1","2","3","4","5","6","7","8","9","10","11","12" }; }
    StringArray unisonItems()  { return { "1","2","3","4","5" }; }
    StringArray subWaveItems() { return { "Sine","Tri","Saw","Square" }; }
    StringArray filterItems()  { return { "LP", "BP", "HP", "NT", "PK" }; }
    StringArray bassModeItems(){ return { "Init", "Reese", "Womp", "Screech", "Warhorn", "Whoop", "Donk", "Tearout", "Laser", "Neuro", "Horn Stab", "Vapor" }; }
    StringArray noiseTypeItems(){ return { "White", "Air", "Metal", "Crunch", "Vowel", "Laser", "Horn", "Vinyl" }; }
    StringArray playItems()    { return { "Mono", "Legato", "Poly" }; }
    StringArray polyItems()    { return { "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16" }; }
    StringArray modSources()   { return { "None", "ENV2", "ENV3", "LFO1", "LFO2", "LFO3", "LFO4", "Velocity" }; }
    StringArray modDests()     { return { "None", "Cutoff", "OSC A Pos", "OSC B Pos", "Pitch", "Level", "Pan", "Dist", "FX" }; }

    Colour brighten(Colour c, float a) { return c.interpolatedWith(Colours::white, a); }

    void drawSection(Graphics& g, Rectangle<float> r, const String& title, Colour accent)
    {
        auto outer = r.expanded(2.0f);
        ColourGradient halo(accent.withAlpha(0.12f), outer.getTopLeft(), accent.withAlpha(0.01f), outer.getBottomRight(), false);
        halo.addColour(0.65, accent.withAlpha(0.05f));
        g.setGradientFill(halo);
        g.fillRoundedRectangle(outer, 18.0f);

        ColourGradient fill(panel1(), r.getTopLeft(), panel0(), r.getBottomRight(), false);
        fill.addColour(0.32, panel2());
        fill.addColour(1.0, Colour(0xff070d18));
        g.setGradientFill(fill);
        g.fillRoundedRectangle(r, 15.0f);

        auto band = r.reduced(1.5f).removeFromTop(34.0f);
        ColourGradient bandFill(accent.withAlpha(0.18f), band.getTopLeft(), accent.withAlpha(0.04f), band.getTopRight(), false);
        bandFill.addColour(1.0, Colours::transparentBlack);
        g.setGradientFill(bandFill);
        g.fillRoundedRectangle(band, 13.0f);

        g.setColour(Colours::white.withAlpha(0.03f));
        for (int y = 0; y < (int) r.getHeight(); y += 14)
            g.drawHorizontalLine((int) r.getY() + y, r.getX() + 10.0f, r.getRight() - 10.0f);

        g.setColour(accent.withAlpha(0.90f));
        g.drawRoundedRectangle(r, 15.0f, 1.15f);
        g.setColour(accent.withAlpha(0.18f));
        g.drawRoundedRectangle(r.reduced(4.0f), 12.0f, 1.0f);

        g.setColour(textHi());
        g.setFont(FontOptions(14.0f, Font::bold));
        g.drawText(title, Rectangle<int>(roundToInt(r.getX() + 14.0f), roundToInt(r.getY() + 7.0f), roundToInt(r.getWidth() - 28.0f), 20), Justification::left);
    }
}

void BeefcakeLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                           float sliderPosProportional, float rotaryStartAngle,
                                           float rotaryEndAngle, Slider& slider)
{
    auto bounds = Rectangle<float>((float) x, (float) y, (float) width, (float) height).reduced(8.0f, 10.0f);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();
    auto arcBounds = Rectangle<float>(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);
    auto accent = slider.findColour(Slider::rotarySliderFillColourId);
    auto accentBright = brighten(accent, 0.25f);
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    DropShadow(accent.withAlpha(0.25f), 12, Point<int>()).drawForRectangle(g, arcBounds.toNearestInt().expanded(4));

    g.setColour(Colours::black.withAlpha(0.30f));
    g.fillEllipse(arcBounds.translated(0.0f, 2.0f));

    ColourGradient face(panel2(), arcBounds.getTopLeft(), panel0(), arcBounds.getBottomRight(), false);
    face.addColour(0.55, panel1());
    g.setGradientFill(face);
    g.fillEllipse(arcBounds);

    g.setColour(Colours::white.withAlpha(0.06f));
    g.drawEllipse(arcBounds.reduced(2.0f), 1.0f);

    Path track;
    track.addCentredArc(centre.x, centre.y, radius - 4.0f, radius - 4.0f, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(Colours::white.withAlpha(0.08f));
    g.strokePath(track, PathStrokeType(4.5f, PathStrokeType::curved, PathStrokeType::rounded));

    Path active;
    active.addCentredArc(centre.x, centre.y, radius - 4.0f, radius - 4.0f, 0.0f, rotaryStartAngle, angle, true);
    g.setColour(accent.withAlpha(0.18f));
    g.strokePath(active, PathStrokeType(9.0f, PathStrokeType::curved, PathStrokeType::rounded));
    g.setColour(accent);
    g.strokePath(active, PathStrokeType(4.2f, PathStrokeType::curved, PathStrokeType::rounded));

    Path pointer;
    auto pointerLength = radius * 0.62f;
    auto pointerThickness = jmax(2.0f, radius * 0.11f);
    pointer.addRoundedRectangle(-pointerThickness * 0.5f, -radius + 7.0f, pointerThickness, pointerLength, pointerThickness * 0.5f);
    g.setColour(accentBright);
    g.fillPath(pointer, AffineTransform::rotation(angle).translated(centre.x, centre.y));

    g.setColour(accentBright.withAlpha(0.65f));
    g.fillEllipse(Rectangle<float>(0.0f, 0.0f, pointerThickness + 2.0f, pointerThickness + 2.0f).withCentre(centre));
}

void BeefcakeLookAndFeel::drawComboBox(Graphics& g, int width, int height, bool,
                                       int buttonX, int buttonY, int buttonW, int buttonH,
                                       ComboBox& box)
{
    auto bounds = Rectangle<float>(0.0f, 0.0f, (float) width, (float) height);
    auto accent = box.findColour(ComboBox::arrowColourId);

    ColourGradient fill(panel2(), bounds.getTopLeft(), panel0(), bounds.getBottomRight(), false);
    fill.addColour(0.55, Colour(0xff101a2d));
    g.setGradientFill(fill);
    g.fillRoundedRectangle(bounds.reduced(0.5f), 6.0f);

    g.setColour(accent.withAlpha(0.95f));
    g.drawRoundedRectangle(bounds.reduced(0.5f), 6.0f, 1.0f);
    g.setColour(Colours::white.withAlpha(0.04f));
    g.drawRoundedRectangle(bounds.reduced(3.0f), 4.0f, 1.0f);

    Path arrow;
    const float cx = (float) buttonX + (float) buttonW * 0.5f;
    const float cy = (float) buttonY + (float) buttonH * 0.5f + 1.0f;
    arrow.startNewSubPath(cx - 5.0f, cy - 2.0f);
    arrow.lineTo(cx, cy + 3.0f);
    arrow.lineTo(cx + 5.0f, cy - 2.0f);
    g.setColour(accent);
    g.strokePath(arrow, PathStrokeType(1.8f));
}

void BeefcakeLookAndFeel::positionComboBoxText(ComboBox& box, Label& label)
{
    label.setBounds(8, 1, box.getWidth() - 24, box.getHeight() - 2);
    label.setFont(getComboBoxFont(box));
    label.setJustificationType(Justification::centredLeft);
    label.setColour(Label::textColourId, textHi());
}

Font BeefcakeLookAndFeel::getComboBoxFont(ComboBox&)
{
    return Font(FontOptions(13.0f));
}

void BeefcakeLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button, bool, bool)
{
    auto r = button.getLocalBounds().toFloat();
    auto box = Rectangle<float>(0.0f, 0.0f, 18.0f, 18.0f).withCentre(Point<float>(r.getX() + 12.0f, r.getCentreY()));
    auto accent = button.findColour(ToggleButton::tickColourId);

    g.setColour(panel2());
    g.fillRoundedRectangle(box, 4.0f);
    g.setColour(accent.withAlpha(0.85f));
    g.drawRoundedRectangle(box, 4.0f, 1.0f);

    if (button.getToggleState())
    {
        g.setColour(accent.withAlpha(0.20f));
        g.fillRoundedRectangle(box.reduced(2.5f), 3.0f);
        g.setColour(accent);
        Path tick;
        tick.startNewSubPath(box.getX() + 4.0f, box.getCentreY());
        tick.lineTo(box.getX() + 8.0f, box.getBottom() - 4.5f);
        tick.lineTo(box.getRight() - 4.0f, box.getY() + 4.5f);
        g.strokePath(tick, PathStrokeType(2.0f));
    }

    g.setColour(button.findColour(ToggleButton::textColourId));
    g.setFont(FontOptions(12.5f));
    g.drawText(button.getButtonText(), Rectangle<int>(26, 0, button.getWidth() - 26, button.getHeight()), Justification::centredLeft);
}

void ScopeDisplay::timerCallback() { processor.copyScopeData(data); repaint(); }
void SpectrumDisplay::timerCallback() { processor.copyAnalyzerData(data); repaint(); }
void LfoDisplay::timerCallback() { processor.copyLfoShape(data); repaint(); }
void MatrixDisplay::timerCallback() { summary = processor.getMatrixSummary(); if (summary.trim().isEmpty() || summary.contains("0.00")) summary = juce::String(R"(Tap RANDOMIZE for fresh basslines
Use Smooth / Reese / Womp / Warhorn as launch pads
Tomorrow test: note-off feel, hero visuals, low-end weight)"); repaint(); }

void ScopeDisplay::paint(Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced(8.0f);
    ColourGradient fill(Colour(0xff050a13), r.getTopLeft(), Colour(0xff0d1830), r.getBottomRight(), false);
    fill.addColour(0.42, Colour(0xff091120));
    fill.addColour(0.84, Colour(0xff070d18));
    g.setGradientFill(fill);
    g.fillRoundedRectangle(r, 12.0f);

    auto hero = r.reduced(14.0f);
    auto caption = hero.removeFromTop(22.0f);
    g.setColour(textMed());
    g.setFont(FontOptions(13.0f, Font::bold));
    g.drawText("3D WAVETABLE VIEW", caption.toNearestInt(), Justification::left);
    g.setColour(textLo());
    g.setFont(FontOptions(11.5f));
    g.drawText("phase lane · spectral body · motion lane · animated depth stack", caption.toNearestInt(), Justification::right);

    auto mainView = hero.removeFromTop(hero.getHeight() * 0.70f);
    auto footer = hero.reduced(0.0f, 2.0f);
    auto specArea = footer.removeFromLeft(footer.getWidth() * 0.62f);
    footer.removeFromLeft(10);
    auto motionArea = footer;

    for (int i = 0; i < 9; ++i)
    {
        const float t = (float) i / 8.0f;
        const float y = mainView.getY() + t * mainView.getHeight();
        g.setColour(Colours::white.withAlpha(i == 4 ? 0.05f : 0.022f));
        g.drawLine(mainView.getX(), y, mainView.getRight(), y, i == 4 ? 1.3f : 1.0f);
    }
    for (int i = 0; i < 16; ++i)
    {
        const float t = (float) i / 15.0f;
        const float x = mainView.getX() + t * mainView.getWidth();
        g.setColour(Colours::white.withAlpha(0.018f));
        g.drawLine(x, mainView.getY() + 8.0f, x, mainView.getBottom() - 8.0f, 1.0f);
    }

    const int layers = 14;
    for (int layer = layers - 1; layer >= 0; --layer)
    {
        const float depth = (float) layer / (float) (layers - 1);
        const float scale = 1.0f - depth * 0.28f;
        const float yOffset = depth * 52.0f;
        const float xInset = depth * 24.0f;
        Path wave;
        for (size_t i = 0; i < data.size(); ++i)
        {
            const float t = (float) i / (float) (data.size() - 1);
            const float x = mainView.getX() + xInset + t * (mainView.getWidth() - xInset * 2.0f);
            const float shimmer = std::sin(t * MathConstants<float>::twoPi * (2.0f + depth * 1.7f)) * (2.0f + 4.0f * (1.0f - depth));
            const float y = mainView.getCentreY() + yOffset - data[i] * (mainView.getHeight() * 0.34f * scale) + shimmer;
            if (i == 0) wave.startNewSubPath(x, y); else wave.lineTo(x, y);
        }
        auto c = displayA().interpolatedWith(displayB(), depth).withAlpha(0.04f + 0.14f * (1.0f - depth));
        g.setColour(c);
        g.strokePath(wave, PathStrokeType(9.0f - depth * 4.0f, PathStrokeType::curved, PathStrokeType::rounded));
    }

    Path front;
    Path fillPath;
    fillPath.startNewSubPath(mainView.getX(), mainView.getBottom());
    for (size_t i = 0; i < data.size(); ++i)
    {
        const float t = (float) i / (float) (data.size() - 1);
        const float x = mainView.getX() + t * mainView.getWidth();
        const float y = mainView.getCentreY() - data[i] * mainView.getHeight() * 0.31f;
        if (i == 0) front.startNewSubPath(x, y); else front.lineTo(x, y);
        fillPath.lineTo(x, y);
    }
    fillPath.lineTo(mainView.getRight(), mainView.getBottom());
    fillPath.closeSubPath();

    ColourGradient area(displayA().withAlpha(0.30f), mainView.getBottomLeft(), displayB().withAlpha(0.05f), mainView.getTopRight(), false);
    area.addColour(0.40, displayB().withAlpha(0.14f));
    area.addColour(0.72, displayA().withAlpha(0.08f));
    g.setGradientFill(area);
    g.fillPath(fillPath);

    g.setColour(displayA().withAlpha(0.22f));
    g.strokePath(front, PathStrokeType(26.0f, PathStrokeType::curved, PathStrokeType::rounded));
    g.setColour(displayB().withAlpha(0.25f));
    g.strokePath(front, PathStrokeType(14.0f, PathStrokeType::curved, PathStrokeType::rounded));
    g.setColour(displayA());
    g.strokePath(front, PathStrokeType(2.8f, PathStrokeType::curved, PathStrokeType::rounded));

    const float beamX = mainView.getX() + 0.70f * mainView.getWidth();
    ColourGradient scan(displayA().withAlpha(0.00f), beamX - 110.0f, mainView.getY(), displayA().withAlpha(0.22f), beamX, mainView.getCentreY(), false);
    scan.addColour(0.45, displayB().withAlpha(0.15f));
    scan.addColour(1.0, displayA().withAlpha(0.00f));
    g.setGradientFill(scan);
    g.fillRoundedRectangle(Rectangle<float>(beamX - 110.0f, mainView.getY() + 8.0f, 220.0f, mainView.getHeight() - 16.0f), 16.0f);

    for (int i = 0; i < 54; ++i)
    {
        const float t = (float) i / 53.0f;
        const float x = mainView.getX() + 14.0f + t * (mainView.getWidth() - 28.0f);
        const float y = mainView.getCentreY() + std::sin(t * MathConstants<float>::twoPi * 4.0f) * 12.0f;
        const float sz = 1.2f + 3.6f * std::abs(std::sin(t * MathConstants<float>::twoPi * 0.75f));
        g.setColour(displayB().interpolatedWith(displayA(), t).withAlpha(0.65f));
        g.fillEllipse(x - sz * 0.5f, y - sz * 0.5f, sz, sz);
    }

    g.setColour(panel0());
    g.fillRoundedRectangle(specArea, 8.0f);
    g.setColour(displayB().withAlpha(0.18f));
    g.drawRoundedRectangle(specArea, 8.0f, 1.0f);

    for (int i = 0; i < 36; ++i)
    {
        const float t = (float) i / 35.0f;
        const float x = specArea.getX() + 12.0f + t * (specArea.getWidth() - 24.0f);
        const float v = 0.18f + 0.72f * std::pow(std::sin(t * MathConstants<float>::pi), 1.5f);
        const float h = (specArea.getHeight() - 18.0f) * v;
        auto c = displayA().interpolatedWith(displayB(), t).withAlpha(0.85f);
        g.setColour(c.withAlpha(0.16f));
        g.fillRoundedRectangle(x - 2.0f, specArea.getBottom() - h - 8.0f, 4.0f, h, 2.0f);
        g.setColour(c);
        g.fillRoundedRectangle(x - 1.0f, specArea.getBottom() - h - 8.0f, 2.0f, h, 1.0f);
    }
    g.setColour(textLo());
    g.drawText("spectral body", specArea.toNearestInt().reduced(10, 6), Justification::topRight);

    g.setColour(panel0());
    g.fillRoundedRectangle(motionArea, 8.0f);
    g.setColour(displayA().withAlpha(0.18f));
    g.drawRoundedRectangle(motionArea, 8.0f, 1.0f);

    Path motion;
    Path motionFill;
    motionFill.startNewSubPath(motionArea.getX(), motionArea.getBottom());
    for (int i = 0; i < 180; ++i)
    {
        const float t = (float) i / 179.0f;
        const float x = motionArea.getX() + t * motionArea.getWidth();
        const float wobble = std::sin(t * MathConstants<float>::twoPi * 1.1f) * 0.24f + std::sin(t * MathConstants<float>::twoPi * 0.33f) * 0.09f;
        const float y = motionArea.getCentreY() - wobble * motionArea.getHeight() * 0.44f;
        if (i == 0) motion.startNewSubPath(x, y); else motion.lineTo(x, y);
        motionFill.lineTo(x, y);
    }
    motionFill.lineTo(motionArea.getRight(), motionArea.getBottom());
    motionFill.closeSubPath();
    ColourGradient motionGrad(displayB().withAlpha(0.22f), motionArea.getBottomLeft(), displayA().withAlpha(0.04f), motionArea.getTopRight(), false);
    g.setGradientFill(motionGrad);
    g.fillPath(motionFill);
    g.setColour(displayB().withAlpha(0.28f));
    g.strokePath(motion, PathStrokeType(7.0f));
    g.setColour(displayB());
    g.strokePath(motion, PathStrokeType(2.0f));
    g.setColour(textLo());
    g.drawText("motion lane", motionArea.toNearestInt().reduced(10, 6), Justification::topRight);
}

void SpectrumDisplay::paint(Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced(4.0f);
    ColourGradient bg(panel0(), r.getTopLeft(), panel1(), r.getBottomRight(), false);
    bg.addColour(0.55, panel2().withAlpha(0.85f));
    g.setGradientFill(bg);
    g.fillRoundedRectangle(r, 8.0f);
    g.setColour(displayB().withAlpha(0.18f));
    g.drawRoundedRectangle(r, 8.0f, 1.0f);

    for (int i = 0; i < 10; ++i)
    {
        const float x = r.getX() + (float) i / 9.0f * r.getWidth();
        g.setColour(Colours::white.withAlpha(0.02f));
        g.drawLine(x, r.getY() + 4.0f, x, r.getBottom() - 4.0f, 1.0f);
    }

    Path line;
    Path fillPath;
    fillPath.startNewSubPath(r.getX(), r.getBottom());
    for (size_t i = 0; i < data.size(); ++i)
    {
        const float x = r.getX() + r.getWidth() * (float) i / (float) (data.size() - 1);
        const float norm = jlimit(0.0f, 1.0f, (data[i] + 100.0f) / 100.0f);
        const float y = r.getBottom() - norm * r.getHeight();
        if (i == 0) line.startNewSubPath(x, y); else line.lineTo(x, y);
        fillPath.lineTo(x, y);
    }
    fillPath.lineTo(r.getRight(), r.getBottom());
    fillPath.closeSubPath();

    ColourGradient fill(displayB().withAlpha(0.24f), r.getBottomLeft(), displayA().withAlpha(0.10f), r.getTopRight(), false);
    fill.addColour(0.5, displayA().withAlpha(0.18f));
    g.setGradientFill(fill);
    g.fillPath(fillPath);

    g.setColour(displayA().withAlpha(0.22f));
    g.strokePath(line, PathStrokeType(8.0f));
    g.setColour(displayB().withAlpha(0.18f));
    g.strokePath(line, PathStrokeType(4.0f));
    g.setColour(displayA());
    g.strokePath(line, PathStrokeType(1.8f));
}

void LfoDisplay::paint(Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced(4.0f);
    ColourGradient bg(panel0(), r.getTopLeft(), panel1(), r.getBottomRight(), false);
    bg.addColour(0.55, panel2().withAlpha(0.82f));
    g.setGradientFill(bg);
    g.fillRoundedRectangle(r, 8.0f);
    g.setColour(subCol().withAlpha(0.18f));
    g.drawRoundedRectangle(r, 8.0f, 1.0f);

    for (int i = 0; i < 8; ++i)
    {
        const float x = r.getX() + (float) i / 7.0f * r.getWidth();
        g.setColour(Colours::white.withAlpha(0.02f));
        g.drawLine(x, r.getY() + 3.0f, x, r.getBottom() - 3.0f, 1.0f);
    }
    g.setColour(Colours::white.withAlpha(0.06f));
    g.drawLine(r.getX(), r.getCentreY(), r.getRight(), r.getCentreY(), 1.0f);

    Path p;
    Path glow;
    for (size_t i = 0; i < data.size(); ++i)
    {
        const float x = r.getX() + r.getWidth() * (float) i / (float) (data.size() - 1);
        const float y = r.getCentreY() - data[i] * r.getHeight() * 0.40f;
        if (i == 0) { p.startNewSubPath(x, y); glow.startNewSubPath(x, y); }
        else { p.lineTo(x, y); glow.lineTo(x, y); }
    }
    g.setColour(subCol().withAlpha(0.22f));
    g.strokePath(glow, PathStrokeType(8.0f));
    g.setColour(displayA().withAlpha(0.18f));
    g.strokePath(glow, PathStrokeType(4.0f));
    g.setColour(subCol());
    g.strokePath(p, PathStrokeType(2.0f));
}

void MatrixDisplay::paint(Graphics& g)
{
    auto r = getLocalBounds().reduced(6).toFloat();
    g.setColour(panel0());
    g.fillRoundedRectangle(r, 8.0f);
    g.setColour(modCol().withAlpha(0.20f));
    g.drawRoundedRectangle(r, 8.0f, 1.0f);
    g.setColour(textLo());
    g.setFont(FontOptions(11.0f));
    g.drawFittedText(summary, r.toNearestInt().reduced(8), Justification::topLeft, 6);
}

SerumKnob::SerumKnob()
{
    addAndMakeVisible(slider);
    addAndMakeVisible(title);
    slider.setSliderStyle(Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 58, 16);
    slider.setColour(Slider::rotarySliderFillColourId, displayA());
    slider.setColour(Slider::thumbColourId, brighten(displayA(), 0.25f));
    slider.setColour(Slider::textBoxTextColourId, textHi());
    slider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    slider.setColour(Slider::textBoxBackgroundColourId, Colours::black.withAlpha(0.20f));
    title.setJustificationType(Justification::centred);
    title.setColour(Label::textColourId, textLo());
    title.setFont(FontOptions(11.0f, Font::plain));
}

void SerumKnob::setAccentColour(Colour colour)
{
    slider.setColour(Slider::rotarySliderFillColourId, colour);
    slider.setColour(Slider::thumbColourId, brighten(colour, 0.22f));
    title.setColour(Label::textColourId, brighten(colour, 0.15f).withAlpha(0.88f));
    if (linearStyle)
    {
        slider.setColour(Slider::trackColourId, colour.withAlpha(0.30f));
        slider.setColour(Slider::backgroundColourId, Colours::black.withAlpha(0.28f));
    }
}

void SerumKnob::setLinearStyle(bool shouldBeLinear)
{
    linearStyle = shouldBeLinear;
    if (linearStyle)
    {
        slider.setSliderStyle(Slider::LinearHorizontal);
        slider.setTextBoxStyle(Slider::TextBoxRight, false, 54, 18);
        title.setJustificationType(Justification::centredLeft);
        title.setFont(FontOptions(11.5f, Font::bold));
        slider.setScrollWheelEnabled(false);
    }
    else
    {
        slider.setSliderStyle(Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(Slider::TextBoxBelow, false, 58, 16);
        title.setJustificationType(Justification::centred);
        title.setFont(FontOptions(11.0f, Font::plain));
    }
}

void SerumKnob::resized()
{
    auto r = getLocalBounds();
    if (linearStyle)
    {
        title.setBounds(r.removeFromLeft(50));
        slider.setBounds(r.reduced(0, 2));
        return;
    }
    title.setBounds(r.removeFromTop(18));
    slider.setBounds(r);
}

FiveParksVST3AudioProcessorEditor::FiveParksVST3AudioProcessorEditor(FiveParksVST3AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), scope(p), spectrum(p), lfoShape(p), matrixView(p)
{
    setLookAndFeel(&beefcakeLookAndFeel);
    setSize(1920, 1080);

    logo.setText("5parks VST Bassline 3", dontSendNotification);
    logo.setColour(Label::textColourId, textHi());
    logo.setFont(FontOptions(28.0f, Font::bold));
    subtitle.setText("flagship bass instrument · preset lab · smoother harmonics · standout hero wavetable visuals", dontSendNotification);
    subtitle.setColour(Label::textColourId, textLo());
    subtitle.setFont(FontOptions(13.0f));
    addAndMakeVisible(logo);
    addAndMakeVisible(subtitle);

    for (auto* c : { &displayBox, &oscABox, &oscBBox, &subBox, &noiseBox, &filterBox, &bassBox, &envBox, &lfoBox, &fxBox, &perfBox, &matrixBox })
        addAndMakeVisible(*c);

    displayBox.addAndMakeVisible(scope);
    displayBox.addAndMakeVisible(spectrum);
    displayBox.addAndMakeVisible(lfoShape);
    matrixBox.addAndMakeVisible(matrixView);

    for (auto* b : { &randomPresetButton, &smoothPresetButton, &reesePresetButton, &wompPresetButton, &warhornPresetButton })
    {
        matrixBox.addAndMakeVisible(*b);
        b->setColour(juce::TextButton::buttonColourId, juce::Colours::black.withAlpha(0.18f));
        b->setColour(juce::TextButton::buttonOnColourId, modCol().withAlpha(0.25f));
        b->setColour(juce::TextButton::textColourOffId, textHi());
        b->setColour(juce::TextButton::textColourOnId, textHi());
    }
    randomPresetButton.setButtonText("RANDOMIZE");
    smoothPresetButton.setButtonText("SMOOTH");
    reesePresetButton.setButtonText("REESE");
    wompPresetButton.setButtonText("WOMP");
    warhornPresetButton.setButtonText("WARHORN");
    randomPresetButton.onClick = [this] { audioProcessor.randomizePatch(); };
    smoothPresetButton.onClick = [this] { audioProcessor.applyPresetStyle(0); };
    reesePresetButton.onClick = [this] { audioProcessor.applyPresetStyle(1); };
    wompPresetButton.onClick = [this] { audioProcessor.applyPresetStyle(2); };
    warhornPresetButton.onClick = [this] { audioProcessor.applyPresetStyle(3); };

    oscABox.addAndMakeVisible(oscAOct); oscABox.addAndMakeVisible(oscASemi); oscABox.addAndMakeVisible(oscAUnison);
    oscBBox.addAndMakeVisible(oscBOct); oscBBox.addAndMakeVisible(oscBSemi); oscBBox.addAndMakeVisible(oscBUnison);

    for (auto* cb : { &oscAOct,&oscASemi,&oscAUnison,&oscBOct,&oscBSemi,&oscBUnison,&subWave,&noiseType,&filterMode,&bassMode,&playMode,&polyphony,&mod1Src,&mod1Dst,&mod2Src,&mod2Dst,&mod3Src,&mod3Dst,&mod4Src,&mod4Dst,&mod5Src,&mod5Dst,&mod6Src,&mod6Dst })
        styleCombo(*cb);

    oscAOct.addItemList(octaveItems(), 1); oscASemi.addItemList(semiItems(), 1); oscAUnison.addItemList(unisonItems(), 1);
    oscBOct.addItemList(octaveItems(), 1); oscBSemi.addItemList(semiItems(), 1); oscBUnison.addItemList(unisonItems(), 1);
    subWave.addItemList(subWaveItems(), 1); noiseType.addItemList(noiseTypeItems(), 1); filterMode.addItemList(filterItems(), 1); bassMode.addItemList(bassModeItems(), 1); playMode.addItemList(playItems(), 1); polyphony.addItemList(polyItems(), 1);
    for (auto* cb : { &mod1Src,&mod2Src,&mod3Src,&mod4Src,&mod5Src,&mod6Src }) cb->addItemList(modSources(), 1);
    for (auto* cb : { &mod1Dst,&mod2Dst,&mod3Dst,&mod4Dst,&mod5Dst,&mod6Dst }) cb->addItemList(modDests(), 1);

    subDirect.setButtonText("Direct Out");
    noiseDirect.setButtonText("Direct Out");
    for (auto* b : { &subDirect, &noiseDirect })
    {
        b->setColour(ToggleButton::textColourId, textLo());
        b->setColour(ToggleButton::tickColourId, subCol());
        b->setColour(ToggleButton::tickDisabledColourId, textLo());
    }

    subBox.addAndMakeVisible(subWave); subBox.addAndMakeVisible(subDirect);
    noiseBox.addAndMakeVisible(noiseType); noiseBox.addAndMakeVisible(noiseDirect);
    filterBox.addAndMakeVisible(filterMode); bassBox.addAndMakeVisible(bassMode);
    perfBox.addAndMakeVisible(playMode); perfBox.addAndMakeVisible(polyphony);

    attach(oscABox, oscAPos, "oscA_pos", "WT POS");
    attach(oscABox, oscALevel, "oscA_level", "LEVEL");
    attach(oscABox, oscADetune, "oscA_detune", "DETUNE");
    attach(oscABox, oscABlend, "oscA_blend", "BLEND");
    attach(oscABox, oscAPan, "oscA_pan", "PAN");

    attach(oscBBox, oscBPos, "oscB_pos", "WT POS");
    attach(oscBBox, oscBLevel, "oscB_level", "LEVEL");
    attach(oscBBox, oscBDetune, "oscB_detune", "DETUNE");
    attach(oscBBox, oscBBlend, "oscB_blend", "BLEND");
    attach(oscBBox, oscBPan, "oscB_pan", "PAN");

    attach(subBox, subLevel, "sub_level", "LEVEL");
    subLevel.setLinearStyle(true);
    attach(noiseBox, noiseLevel, "noise_level", "LEVEL");
    noiseLevel.setLinearStyle(true);
    attach(filterBox, filterCutoff, "filter_cutoff", "CUTOFF");
    attach(filterBox, filterRes, "filter_res", "RES");
    attach(filterBox, filterDrive, "filter_drive", "DRIVE");
    attach(filterBox, filterMix, "filter_mix", "MIX");

    attach(bassBox, reeseAmt, "reese_amt", "REESE");
    attach(bassBox, warhornAmt, "warhorn_amt", "WARHORN");
    attach(bassBox, wompAmt, "womp_amt", "WOMP");
    attach(bassBox, subharmAmt, "subharm_amt", "SUB HARM");
    attach(bassBox, comboAmt, "combo_amt", "COMBO");
    attach(bassBox, stereoWidth, "stereo_width", "WIDTH");
    attach(bassBox, fmAmt, "fm_amt", "FM GRIT");
    attach(bassBox, airAmt, "air_amt", "AIR");
    attach(bassBox, bodyAmt, "body_amt", "BODY");
    attach(bassBox, growlAmt, "growl_amt", "GROWL");
    attach(bassBox, toneAmt, "tone_amt", "TONE");
    attach(bassBox, motionAmt, "motion_amt", "MOTION");
    attach(bassBox, screechAmt, "screech_amt", "SCREECH");
    attach(bassBox, whoopAmt, "whoop_amt", "WHOOP");
    attach(bassBox, vowelAmt, "vowel_amt", "VOWEL");
    attach(bassBox, smearAmt, "smear_amt", "SMEAR");

    attach(envBox, env1A, "env1_a", "ATTACK");
    attach(envBox, env1D, "env1_d", "DECAY");
    attach(envBox, env1S, "env1_s", "SUSTAIN");
    attach(envBox, env1R, "env1_r", "RELEASE");

    attach(lfoBox, lfo1Rate, "lfo1_rate", "RATE");
    attach(lfoBox, lfo1Amt, "lfo1_amt", "AMOUNT");
    attach(lfoBox, lfo1Shape, "lfo1_shape", "SHAPE");
    attach(lfoBox, lfo1Smooth, "lfo1_smooth", "SMOOTH");

    attach(fxBox, distDrive, "dist_drive", "DIST");
    attach(fxBox, distMix, "dist_mix", "MIX");
    attach(fxBox, chorusMix, "chorus_mix", "CHORUS");
    attach(fxBox, delayMix, "delay_mix", "DELAY");
    attach(fxBox, reverbMix, "reverb_mix", "REVERB");
    attach(fxBox, compAmt, "comp_amt", "COMP");

    attach(perfBox, glide, "glide_ms", "GLIDE");
    attach(perfBox, masterGain, "master_gain", "MASTER");

    addMatrixSlot(1, mod1Src, mod1Dst, mod1Amt, "mod1_src", "mod1_dst", "mod1_amt");
    addMatrixSlot(2, mod2Src, mod2Dst, mod2Amt, "mod2_src", "mod2_dst", "mod2_amt");
    addMatrixSlot(3, mod3Src, mod3Dst, mod3Amt, "mod3_src", "mod3_dst", "mod3_amt");
    addMatrixSlot(4, mod4Src, mod4Dst, mod4Amt, "mod4_src", "mod4_dst", "mod4_amt");
    addMatrixSlot(5, mod5Src, mod5Dst, mod5Amt, "mod5_src", "mod5_dst", "mod5_amt");
    addMatrixSlot(6, mod6Src, mod6Dst, mod6Amt, "mod6_src", "mod6_dst", "mod6_amt");

    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "oscA_oct", oscAOct));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "oscA_semi", oscASemi));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "oscA_unison", oscAUnison));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "oscB_oct", oscBOct));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "oscB_semi", oscBSemi));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "oscB_unison", oscBUnison));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "sub_wave", subWave));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "noise_type", noiseType));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "filter_mode", filterMode));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "bass_mode", bassMode));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "play_mode", playMode));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, "polyphony", polyphony));
    buttonAtts.push_back(std::make_unique<ButtonAttachment>(audioProcessor.apvts, "sub_direct", subDirect));
    buttonAtts.push_back(std::make_unique<ButtonAttachment>(audioProcessor.apvts, "noise_direct", noiseDirect));

    tintKnobGroup({ &oscAPos, &oscALevel, &oscADetune, &oscABlend, &oscAPan }, oscACol());
    tintKnobGroup({ &oscBPos, &oscBLevel, &oscBDetune, &oscBBlend, &oscBPan }, oscBCol());
    tintKnobGroup({ &subLevel, &noiseLevel }, subCol());
    tintKnobGroup({ &filterCutoff, &filterRes, &filterDrive, &filterMix }, filterCol());
    tintKnobGroup({ &reeseAmt, &warhornAmt, &wompAmt, &subharmAmt, &comboAmt, &stereoWidth, &fmAmt, &airAmt,
                    &bodyAmt, &growlAmt, &toneAmt, &motionAmt, &screechAmt, &whoopAmt, &vowelAmt, &smearAmt }, bassCol());
    tintKnobGroup({ &env1A, &env1D, &env1S, &env1R }, envCol());
    tintKnobGroup({ &lfo1Rate, &lfo1Amt, &lfo1Shape, &lfo1Smooth }, lfoCol());
    tintKnobGroup({ &distDrive, &distMix, &chorusMix, &delayMix, &reverbMix, &compAmt }, fxCol());
    tintKnobGroup({ &glide, &masterGain }, perfCol());
    tintKnobGroup({ &mod1Amt, &mod2Amt, &mod3Amt, &mod4Amt, &mod5Amt, &mod6Amt }, modCol());
}

FiveParksVST3AudioProcessorEditor::~FiveParksVST3AudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void FiveParksVST3AudioProcessorEditor::configureKnob(SerumKnob& knob, const String& title)
{
    knob.title.setText(title, dontSendNotification);
}

void FiveParksVST3AudioProcessorEditor::attach(Component& parent, SerumKnob& knob, const String& id, const String& title)
{
    configureKnob(knob, title);
    parent.addAndMakeVisible(knob);
    sliderAtts.push_back(std::make_unique<SliderAttachment>(audioProcessor.apvts, id, knob.slider));
}

void FiveParksVST3AudioProcessorEditor::styleCombo(ComboBox& box)
{
    box.setColour(ComboBox::backgroundColourId, Colours::transparentBlack);
    box.setColour(ComboBox::outlineColourId, displayA().withAlpha(0.8f));
    box.setColour(ComboBox::textColourId, textHi());
    box.setColour(ComboBox::arrowColourId, displayA());
}

void FiveParksVST3AudioProcessorEditor::addMatrixSlot(int, ComboBox& src, ComboBox& dst, SerumKnob& amt, const String& srcId, const String& dstId, const String& amtId)
{
    matrixBox.addAndMakeVisible(src);
    matrixBox.addAndMakeVisible(dst);
    attach(matrixBox, amt, amtId, "AMT");
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, srcId, src));
    comboAtts.push_back(std::make_unique<ComboAttachment>(audioProcessor.apvts, dstId, dst));
}

void FiveParksVST3AudioProcessorEditor::tintKnobGroup(std::initializer_list<SerumKnob*> knobs, Colour colour)
{
    for (auto* knob : knobs)
        if (knob != nullptr)
            knob->setAccentColour(colour);
}

void FiveParksVST3AudioProcessorEditor::paint(Graphics& g)
{
    ColourGradient bg(bg0(), 0.0f, 0.0f, bg2(), 0.0f, (float) getHeight(), false);
    bg.addColour(0.38, bg1());
    bg.addColour(1.0, Colour(0xff03050a));
    g.setGradientFill(bg);
    g.fillAll();

    Path beamA;
    beamA.startNewSubPath(220.0f, 0.0f);
    beamA.lineTo((float) getWidth() * 0.72f, 0.0f);
    beamA.lineTo((float) getWidth() * 0.44f, (float) getHeight() * 0.42f);
    beamA.lineTo(0.0f, (float) getHeight() * 0.42f);
    beamA.closeSubPath();
    g.setColour(displayA().withAlpha(0.045f));
    g.fillPath(beamA);

    Path beamB;
    beamB.startNewSubPath((float) getWidth(), 90.0f);
    beamB.lineTo((float) getWidth(), (float) getHeight() * 0.55f);
    beamB.lineTo((float) getWidth() * 0.60f, (float) getHeight() * 0.30f);
    beamB.closeSubPath();
    g.setColour(displayB().withAlpha(0.05f));
    g.fillPath(beamB);

    g.setColour(Colours::white.withAlpha(0.03f));
    for (int y = 0; y < getHeight(); y += 32)
        g.drawHorizontalLine(y, 0.0f, (float) getWidth());
    for (int x = 0; x < getWidth(); x += 32)
        g.drawVerticalLine(x, 0.0f, (float) getHeight());

    drawSection(g, displayBox.getBounds().toFloat(), "WAVETABLE HERO · 3D WAVE · SCOPE · SPECTRUM", displayA());
    drawSection(g, oscABox.getBounds().toFloat(), "OSC A", oscACol());
    drawSection(g, oscBBox.getBounds().toFloat(), "OSC B", oscBCol());
    drawSection(g, subBox.getBounds().toFloat(), "SUB", subCol());
    drawSection(g, noiseBox.getBounds().toFloat(), "NOISE", subCol());
    drawSection(g, filterBox.getBounds().toFloat(), "FILTER", filterCol());
    drawSection(g, bassBox.getBounds().toFloat(), "BASS ENGINE", bassCol());
    drawSection(g, envBox.getBounds().toFloat(), "AMP ENV", envCol());
    drawSection(g, lfoBox.getBounds().toFloat(), "LFO · MOTION", lfoCol());
    drawSection(g, fxBox.getBounds().toFloat(), "FX", fxCol());
    drawSection(g, perfBox.getBounds().toFloat(), "PERFORMANCE", perfCol());
    drawSection(g, matrixBox.getBounds().toFloat(), "PRESET LAB", modCol());

    auto drawComboHint = [&](juce::Component& parent, juce::ComboBox& box, const juce::String& text, juce::Colour c)
    {
        auto b = box.getBounds().translated(parent.getX(), parent.getY());
        g.setColour(c.withAlpha(0.82f));
        g.setFont(FontOptions(10.5f, Font::bold));
        g.drawText(text, Rectangle<int>(b.getX(), b.getY() - 14, b.getWidth(), 12), Justification::centred);
    };

    drawComboHint(oscABox, oscAOct, "OCT", oscACol());
    drawComboHint(oscABox, oscASemi, "SEMI", oscACol());
    drawComboHint(oscABox, oscAUnison, "UNI", oscACol());
    drawComboHint(oscBBox, oscBOct, "OCT", oscBCol());
    drawComboHint(oscBBox, oscBSemi, "SEMI", oscBCol());
    drawComboHint(oscBBox, oscBUnison, "UNI", oscBCol());

    auto strip = Rectangle<float>(20.0f, (float) getHeight() - 18.0f, (float) getWidth() - 40.0f, 5.0f);
    ColourGradient stripFill(displayA().withAlpha(0.20f), strip.getX(), strip.getCentreY(), displayB().withAlpha(0.20f), strip.getRight(), strip.getCentreY(), false);
    stripFill.addColour(0.5, subCol().withAlpha(0.24f));
    g.setGradientFill(stripFill);
    g.fillRoundedRectangle(strip, 2.0f);
}

void FiveParksVST3AudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(18);
    auto header = area.removeFromTop(58);
    logo.setBounds(header.removeFromLeft(560));
    subtitle.setBounds(header);
    area.removeFromTop(10);

    auto hero = area.removeFromTop(420);
    displayBox.setBounds(hero);

    area.removeFromTop(10);
    auto row1 = area.removeFromTop(246);
    oscABox.setBounds(row1.removeFromLeft(320));
    row1.removeFromLeft(10);
    oscBBox.setBounds(row1.removeFromLeft(320));
    row1.removeFromLeft(10);
    filterBox.setBounds(row1.removeFromLeft(220));
    row1.removeFromLeft(10);
    auto stackBox = row1.removeFromLeft(380);
    subBox.setBounds(stackBox.removeFromTop(118));
    stackBox.removeFromTop(10);
    noiseBox.setBounds(stackBox);
    row1.removeFromLeft(10);
    perfBox.setBounds(row1);

    area.removeFromTop(10);
    auto row2 = area;
    bassBox.setBounds(row2.removeFromLeft(700));
    row2.removeFromLeft(10);
    envBox.setBounds(row2.removeFromLeft(250));
    row2.removeFromLeft(10);
    lfoBox.setBounds(row2.removeFromLeft(250));
    row2.removeFromLeft(10);
    fxBox.setBounds(row2.removeFromLeft(250));
    row2.removeFromLeft(10);
    matrixBox.setBounds(row2);

    auto disp = displayBox.getLocalBounds().reduced(12);
    disp.removeFromTop(28);
    auto heroWave = disp.removeFromTop(250);
    scope.setBounds(heroWave);
    disp.removeFromTop(8);
    auto lower = disp.removeFromTop(64);
    spectrum.setBounds(lower.removeFromLeft((int) (lower.getWidth() * 0.62f)));
    lower.removeFromLeft(8);
    lfoShape.setBounds(lower);

    auto layoutOsc = [](Component& box, ComboBox& oct, ComboBox& semi, ComboBox& uni, std::initializer_list<SerumKnob*> knobs)
    {
        auto r = box.getLocalBounds().reduced(10); r.removeFromTop(28);
        auto head = r.removeFromTop(26);
        oct.setBounds(head.removeFromLeft(82)); head.removeFromLeft(8);
        semi.setBounds(head.removeFromLeft(82)); head.removeFromLeft(8);
        uni.setBounds(head.removeFromLeft(82));
        r.removeFromTop(10);
        auto row = r.removeFromTop(152);
        for (auto* k : knobs)
        {
            k->setBounds(row.removeFromLeft(56).reduced(1));
            row.removeFromLeft(4);
        }
    };
    layoutOsc(oscABox, oscAOct, oscASemi, oscAUnison, { &oscAPos, &oscALevel, &oscADetune, &oscABlend, &oscAPan });
    layoutOsc(oscBBox, oscBOct, oscBSemi, oscBUnison, { &oscBPos, &oscBLevel, &oscBDetune, &oscBBlend, &oscBPan });

    auto sb = subBox.getLocalBounds().reduced(10); sb.removeFromTop(28);
    auto srow = sb.removeFromTop(24);
    subWave.setBounds(srow.removeFromLeft(112)); srow.removeFromLeft(8); subDirect.setBounds(srow.removeFromLeft(120));
    sb.removeFromTop(8);
    subLevel.setBounds(sb.removeFromTop(58));

    auto nb = noiseBox.getLocalBounds().reduced(10); nb.removeFromTop(28);
    auto nrow = nb.removeFromTop(24);
    noiseType.setBounds(nrow.removeFromLeft(118)); nrow.removeFromLeft(8); noiseDirect.setBounds(nrow.removeFromLeft(120));
    nb.removeFromTop(8);
    noiseLevel.setBounds(nb.removeFromTop(58));

    auto fb = filterBox.getLocalBounds().reduced(10); fb.removeFromTop(28);
    filterMode.setBounds(fb.removeFromTop(24).removeFromLeft(88));
    fb.removeFromTop(10);
    auto frow = fb.removeFromTop(152);
    for (auto* k : { &filterCutoff, &filterRes, &filterDrive, &filterMix })
    {
        k->setBounds(frow.removeFromLeft(72).reduced(1));
        frow.removeFromLeft(4);
    }

    auto bb = bassBox.getLocalBounds().reduced(10); bb.removeFromTop(28);
    bassMode.setBounds(bb.removeFromTop(24).removeFromLeft(150));
    bb.removeFromTop(8);
    auto brow1 = bb.removeFromTop(76);
    for (auto* k : { &reeseAmt, &warhornAmt, &wompAmt, &subharmAmt }) { k->setBounds(brow1.removeFromLeft(104).reduced(1)); brow1.removeFromLeft(4); }
    bb.removeFromTop(4);
    auto brow2 = bb.removeFromTop(76);
    for (auto* k : { &comboAmt, &stereoWidth, &fmAmt, &airAmt }) { k->setBounds(brow2.removeFromLeft(104).reduced(1)); brow2.removeFromLeft(4); }
    bb.removeFromTop(4);
    auto brow3 = bb.removeFromTop(76);
    for (auto* k : { &bodyAmt, &growlAmt, &toneAmt, &motionAmt }) { k->setBounds(brow3.removeFromLeft(104).reduced(1)); brow3.removeFromLeft(4); }
    bb.removeFromTop(4);
    auto brow4 = bb.removeFromTop(76);
    for (auto* k : { &screechAmt, &whoopAmt, &vowelAmt, &smearAmt }) { k->setBounds(brow4.removeFromLeft(104).reduced(1)); brow4.removeFromLeft(4); }

    auto eb = envBox.getLocalBounds().reduced(10); eb.removeFromTop(28);
    auto erow = eb.removeFromTop(150);
    for (auto* k : { &env1A, &env1D, &env1S, &env1R }) { k->setBounds(erow.removeFromLeft(70).reduced(1)); erow.removeFromLeft(4); }

    auto lb = lfoBox.getLocalBounds().reduced(10); lb.removeFromTop(28);
    auto lrow = lb.removeFromTop(150);
    for (auto* k : { &lfo1Rate, &lfo1Amt, &lfo1Shape, &lfo1Smooth }) { k->setBounds(lrow.removeFromLeft(70).reduced(1)); lrow.removeFromLeft(4); }

    auto xb = fxBox.getLocalBounds().reduced(10); xb.removeFromTop(28);
    auto xrow = xb.removeFromTop(150);
    for (auto* k : { &distDrive, &distMix, &chorusMix, &delayMix, &reverbMix, &compAmt }) { k->setBounds(xrow.removeFromLeft(84).reduced(1)); xrow.removeFromLeft(4); }

    auto pb = perfBox.getLocalBounds().reduced(10); pb.removeFromTop(28);
    auto prow = pb.removeFromTop(24);
    playMode.setBounds(prow.removeFromLeft(120)); prow.removeFromLeft(8);
    polyphony.setBounds(prow.removeFromLeft(110));
    pb.removeFromTop(12);
    auto pkn = pb.removeFromTop(150);
    glide.setBounds(pkn.removeFromLeft(88).reduced(1));
    pkn.removeFromLeft(4);
    masterGain.setBounds(pkn.removeFromLeft(88).reduced(1));

    auto mb = matrixBox.getLocalBounds().reduced(12); mb.removeFromTop(28);
    auto topButton = mb.removeFromTop(48);
    randomPresetButton.setBounds(topButton);
    mb.removeFromTop(10);
    auto rowA = mb.removeFromTop(36);
    smoothPresetButton.setBounds(rowA.removeFromLeft((rowA.getWidth() - 10) / 2));
    rowA.removeFromLeft(10);
    reesePresetButton.setBounds(rowA);
    mb.removeFromTop(8);
    auto rowB = mb.removeFromTop(36);
    wompPresetButton.setBounds(rowB.removeFromLeft((rowB.getWidth() - 10) / 2));
    rowB.removeFromLeft(10);
    warhornPresetButton.setBounds(rowB);
    mb.removeFromTop(12);
    matrixView.setBounds(mb.removeFromTop(140));
    for (auto* cb : { &mod1Src, &mod1Dst, &mod2Src, &mod2Dst, &mod3Src, &mod3Dst, &mod4Src, &mod4Dst, &mod5Src, &mod5Dst, &mod6Src, &mod6Dst }) cb->setBounds(0, 0, 0, 0);
    for (auto* k : { &mod1Amt, &mod2Amt, &mod3Amt, &mod4Amt, &mod5Amt, &mod6Amt }) k->setBounds(0, 0, 0, 0);
}
