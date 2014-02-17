/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/InputSource.h"
#include "chronotext/utils/Utils.h"

#include "cinder/Xml.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace zf;

const float LINE_TOP = 66;
const float LINE_SPACING = 66;

const int LINE_COUNT = 11;
const int MAX_SENTENCES_PER_LINE = 3;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        fontManager.discardTextures(); // NECESSARY ON ANDROID (AFTER OPENGL CONTEXT-LOSS...)
    }
    else
    {
        try
        {
            fontManager.loadConfig(InputSource::getResource("Fonts.xml")); // CAN THROW
        }
        catch (exception &e)
        {
            LOGI << "CAN'T INITIALIZE FontManager | " << e.what() << endl;
        }
        
        // ---
        
        XmlTree doc(InputSource::loadResource("Text.xml"));
        auto rootElement = doc.getChild("Text");
        
        for (auto &lineElement : rootElement.getChildren())
        {
            auto text = trimText(lineElement->getValue()); // WE'RE NOT USING THE lang ATTRIBUTE
            sentences.push_back(text);
        }
        
        // ---
        
        shuffleLines();
        
        fontSize = 27;
        align = ZFont::ALIGN_BASELINE;
        oscillate = true; // TOGGLE BY PRESSING SPACE ON THE DESKTOP
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::update()
{
    if (oscillate)
    {
        fontSize = 27 + 15 * math<float>::sin(getElapsedSeconds() * 3); // OSCILLATING BETWEEN 12 AND 42
    }
}

void Sketch::draw()
{
    gl::clear(Color::gray(0.5f), false);
    
    Vec2i windowSize = getWindowSize();
    gl::setMatricesWindow(windowSize, true);
    
    // ---
    
    drawHLines(lines.size(), LINE_TOP, LINE_SPACING);

    float y = LINE_TOP;
    float left = 24;
    float right = windowSize.x - 24;

    try
    {
        auto font = fontManager.getCachedFont("sans-serif"); // CAN THROW
        
        font->setSize(fontSize);
        font->setColor(1, 1, 1, 0.75f);
        
        font->begin();
        
        for (auto &line : lines)
        {
            drawTextLine(*font, line, y, left, right);
            y += LINE_SPACING;
        }
        
        font->end();
    }
    catch (exception &e)
    {
        LOGI_ONCE("CAN'T GET VirtualFont | " + string(e.what()));
    }
}

void Sketch::drawTextLine(ZFont &font, const string &text, float y, float left, float right)
{
    auto layout = font.getCachedLineLayout(text);
    
    float x = (layout->overallDirection == HB_DIRECTION_RTL) ? (right - font.getAdvance(*layout)) : left;
    Vec2f position(x, y + font.getOffsetY(*layout, align));

    for (auto &cluster : layout->clusters)
    {
        font.addCluster(cluster, position);
        position.x += font.getAdvance(cluster);
    }
}

void Sketch::drawHLines(int count, float top, float spacing)
{
    glColor4f(1, 0.75f, 0, 0.25f);
    
    for (int i = 0; i < count; i++)
    {
        drawHLine(top + spacing * i);
    }
}

void Sketch::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

void Sketch::shuffleLines()
{
    lines.clear();
    
    for (int i = 0; i < LINE_COUNT; i++)
    {
        string line;
        int sentenceCount = rnd.nextInt(1, MAX_SENTENCES_PER_LINE);
        
        for (int j = 0; j < sentenceCount; j++)
        {
            line += sentences[rnd.nextInt(sentences.size())];
            line += " ";
        }
        
        lines.push_back(line);
    }
}

string Sketch::trimText(const string &text)
{
    auto rawLines = split(text, '\n');
    
    for (auto &line : rawLines)
    {
        auto trimmed = boost::algorithm::trim_copy(line);
        
        if (!trimmed.empty())
        {
            return trimmed;
        }
    }
    
    return "";
}
