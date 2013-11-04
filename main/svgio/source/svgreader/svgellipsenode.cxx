/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_svgio.hxx"

#include <svgio/svgreader/svgellipsenode.hxx>
#include <basegfx/polygon/b2dpolygon.hxx>
#include <basegfx/polygon/b2dpolygontools.hxx>

//////////////////////////////////////////////////////////////////////////////

namespace svgio
{
    namespace svgreader
    {
        SvgEllipseNode::SvgEllipseNode(
            SvgDocument& rDocument,
            SvgNode* pParent)
        :   SvgNode(SVGTokenEllipse, rDocument, pParent),
            maSvgStyleAttributes(*this),
            maCx(0),
            maCy(0),
            maRx(0),
            maRy(0),
            mpaTransform(0)
        {
        }

        SvgEllipseNode::~SvgEllipseNode()
        {
            if(mpaTransform) delete mpaTransform;
        }

        const SvgStyleAttributes* SvgEllipseNode::getSvgStyleAttributes() const
        {
            static rtl::OUString aClassStr(rtl::OUString::createFromAscii("ellipse"));

            return checkForCssStyle(aClassStr, maSvgStyleAttributes);
        }

        void SvgEllipseNode::parseAttribute(const rtl::OUString& rTokenName, SVGToken aSVGToken, const rtl::OUString& aContent)
        {
            // call parent
            SvgNode::parseAttribute(rTokenName, aSVGToken, aContent);

            // read style attributes
            maSvgStyleAttributes.parseStyleAttribute(rTokenName, aSVGToken, aContent);

            // parse own
            switch(aSVGToken)
            {
                case SVGTokenStyle:
                {
                    maSvgStyleAttributes.readStyle(aContent);
                    break;
                }
                case SVGTokenCx:
                {
                    SvgNumber aNum;

                    if(readSingleNumber(aContent, aNum))
                    {
                        setCx(aNum);
                    }
                    break;
                }
                case SVGTokenCy:
                {
                    SvgNumber aNum;

                    if(readSingleNumber(aContent, aNum))
                    {
                        setCy(aNum);
                    }
                    break;
                }
                case SVGTokenRx:
                {
                    SvgNumber aNum;

                    if(readSingleNumber(aContent, aNum))
                    {
                        if(aNum.isPositive())
                        {
                            setRx(aNum);
                        }
                    }
                    break;
                }
                case SVGTokenRy:
                {
                    SvgNumber aNum;

                    if(readSingleNumber(aContent, aNum))
                    {
                        if(aNum.isPositive())
                        {
                            setRy(aNum);
                        }
                    }
                    break;
                }
                case SVGTokenTransform:
                {
                    const basegfx::B2DHomMatrix aMatrix(readTransform(aContent, *this));

                    if(!aMatrix.isIdentity())
                    {
                        setTransform(&aMatrix);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        void SvgEllipseNode::decomposeSvgNode(drawinglayer::primitive2d::Primitive2DSequence& rTarget, bool /*bReferenced*/) const
        {
            const SvgStyleAttributes* pStyle = getSvgStyleAttributes();

            if(pStyle && getRx().isSet() && getRy().isSet())
            {
                const double fRx(getRx().solve(*this, xcoordinate));
                const double fRy(getRy().solve(*this, ycoordinate));

                if(fRx > 0.0 && fRy > 0.0)
                {
                    const basegfx::B2DPolygon aPath(
                        basegfx::tools::createPolygonFromEllipse(
                            basegfx::B2DPoint(
                                getCx().isSet() ? getCx().solve(*this, xcoordinate) : 0.0,
                                getCy().isSet() ? getCy().solve(*this, ycoordinate) : 0.0),
                            fRx, fRy));

                    drawinglayer::primitive2d::Primitive2DSequence aNewTarget;

                    pStyle->add_path(basegfx::B2DPolyPolygon(aPath), aNewTarget, 0);
                    
                    if(aNewTarget.hasElements())
                    {
                        pStyle->add_postProcess(rTarget, aNewTarget, getTransform());
                    }
                }
            }
        }
    } // end of namespace svgreader
} // end of namespace svgio

//////////////////////////////////////////////////////////////////////////////
// eof
