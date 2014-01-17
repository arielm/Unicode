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

const float LINE_TOP = 66;
const float LINE_SPACING = 66;

const int LINE_COUNT = 11;
const int MAX_SENTENCES_PER_LINE = 3;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate),
font(NULL)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        fontManager.discardTextures(); // NECESSARY ON ANDROID (AFTER OPENGL CONTEXT-LOSS...)
    }
    else
    {
        fontManager.loadGlobalMap(InputSource::getResource("Fonts.xml"));
        
        try
        {
            font = fontManager.getFont("sans-serif", VirtualFont::STYLE_REGULAR, 27);
        }
        catch (exception &e)
        {
            LOGD << "CAN'T LOAD FONT: " << e.what() << endl;
        }
        
        // ---
        
        XmlTree doc(InputSource::loadResource("Text.xml"));
        auto rootElement = doc.getChild("Text");
        
        for (auto &lineElement : rootElement.getChildren())
        {
            auto text = trimText(lineElement->getValue());
            sentences.push_back(text);
        }
        
//      Measurement(font).run(sentences, LINE_COUNT, MAX_SENTENCES_PER_LINE);
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::draw()
{
    gl::clear(Color::gray(0.5f), false);
    
    Vec2i windowSize = getWindowSize();
    gl::setMatricesWindow(windowSize, true);
    
    rnd.seed(123);
    
    if (font)
    {
        float y = LINE_TOP;
        float left = 24;
        float right = windowSize.x - 24;
        
        font->setColor(ColorA(1, 1, 1, 0.75f));
        
        for (int i = 0; i < LINE_COUNT; i++)
        {
            string line;
            int sentenceCount = rnd.nextInt(1, MAX_SENTENCES_PER_LINE);
            
            for (int j = 0; j < sentenceCount; j++)
            {
                line += sentences[rnd.nextInt(sentences.size())];
                line += " ";
            }
            
            drawLineLayout(*fontManager.layoutCache.getLineLayout(font, line), y, left, right);
            y += LINE_SPACING;
        }
    }
}

void Sketch::drawLineLayout(LineLayout &layout, float y, float left, float right)
{
    float x = (layout.overallDirection == HB_DIRECTION_LTR) ? left : (right - font->getAdvance(layout));
    Vec2f position(x, y);
    
    font->begin();
    
    for (auto cluster : layout.clusters)
    {
        font->drawCluster(cluster, position);
        position.x += font->getAdvance(cluster);
    }
    
    font->end();
    
    // ---
    
    glColor4f(1, 0.75f, 0, 0.25f);
    drawHLine(y);
}

void Sketch::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

string Sketch::trimText(const string &text)
{
    auto rawLines = split(text, '\n');
    
    for (auto line : rawLines)
    {
        auto trimmed = boost::algorithm::trim_copy(line);
        
        if (!trimmed.empty())
        {
            return trimmed;
        }
    }
    
    return "";
}
