#include <cassert>

#include "include/libAn_AnnoRect.h"
#include "include/libAn_XmlHelpers.h"
#include <cmath>
#include <cstdlib>

using namespace std;

namespace libAn {

    ///
///
///   AnnoRect
///
///
//////////////////////////////////////////////////////////////////////////////

// AnnoRect::AnnoRect(const AnnoRect& other)
// {
//   m_x1 = other.m_x1;
//   m_y1 = other.m_y1;
//   m_x2 = other.m_x2;
//   m_y2 = other.m_y2;

//   m_dScore = other.m_dScore;
//   m_nSilhouetteID = other.m_nSilhouetteID;
//   m_dScale = other.m_dScale;
//   m_nObjPosX = other.m_nObjPosX;
//   m_nObjPosY = other.m_nObjPosY;

//   m_vArticulations.insert(m_vArticulations.begin(),
//                           other.m_vArticulations.begin(),
//                           other.m_vArticulations.end());

//   m_vViewPoints.insert(m_vViewPoints.begin(),
//                        other.m_vViewPoints.begin(),
//                        other.m_vViewPoints.end());

//   m_vAnnoPoints.insert(m_vAnnoPoints.begin(),
//                        other.m_vAnnoPoints.begin(),
//                        other.m_vAnnoPoints.end());


//   m_x1 = other.m_x1;
//   m_y1 = other.m_y1;
//   m_x2 = other.m_x2;
//   m_y2 = other.m_y2;

//   m_dScore = other.m_dScore;
//   m_nSilhouetteID = other.m_nSilhouetteID;

//   m_vArticulations.clear();
//   for (unsigned i=0; i<other.noArticulations(); i++)
//   {
//     this->addArticulation(other.articulation(i));
//   }
//   m_vViewPoints.clear();
//   for (unsigned i=0; i<other.noViewPoints(); i++)
//   {
//     this->addViewPoint(other.viewPoint(i));
//   }

//}

    const AnnoPoint &AnnoRect::getAnnoPoint(int nAnnoPointIdx) const {
        assert(nAnnoPointIdx >= 0 && (unsigned)nAnnoPointIdx < m_vAnnoPoints.size());
        assert(nAnnoPointIdx + 1 == m_vAnnoPoints[nAnnoPointIdx].id ||
               nAnnoPointIdx == m_vAnnoPoints[nAnnoPointIdx].id);
        return m_vAnnoPoints[nAnnoPointIdx];
    }

    void AnnoRect::parseXML(const string &rectString) {
        //cerr << "AnnoRect::parse()"<< endl;
        vector<string> tmp;
        tmp = getElements("x1", rectString);
        if (tmp.size() > 0) {
            m_x1 = getElementDataInt("x1", tmp[0]);
        }
        tmp = getElements("x2", rectString);
        if (tmp.size() > 0) {
            m_x2 = getElementDataInt("x2", tmp[0]);
        }
        tmp = getElements("y1", rectString);
        if (tmp.size() > 0) {
            m_y1 = getElementDataInt("y1", tmp[0]);
        }
        tmp = getElements("y2", rectString);
        if (tmp.size() > 0) {
            m_y2 = getElementDataInt("y2", tmp[0]);
        }

        tmp = getElements("x3", rectString);
        if (tmp.size() > 0) {
            m_x3 = getElementDataInt("x3", tmp[0]);
        }

        tmp = getElements("y3", rectString);
        if (tmp.size() > 0) {
            m_y3 = getElementDataInt("y3", tmp[0]);
        }

        tmp = getElements("x4", rectString);
        if (tmp.size() > 0) {
            m_x4 = getElementDataInt("x4", tmp[0]);
        }

        tmp = getElements("y4", rectString);
        if (tmp.size() > 0) {
            m_y4 = getElementDataInt("y4", tmp[0]);
        }

        tmp = getElements("score", rectString);
        if (tmp.size() > 0) {
            m_dScore = getElementDataFloat("score", tmp[0]);
        }

        tmp = getElements("track_id", rectString);
        if (tmp.size() > 0) {
            m_nTrackID = getElementDataInt("track_id", tmp[0]);
        }

        tmp = getElements("silhouette", rectString);
        if (tmp.size() > 0) {
            tmp = getElements("id", tmp[0]);
            for(unsigned i = 0; i < tmp.size(); i++) {
                m_nSilhouetteID = getElementDataInt("id", tmp[0]);
            }
        }

        tmp = getElements("articulation", rectString);
        if (tmp.size() > 0) {
            tmp = getElements("id", tmp[0]);
            for(unsigned i = 0; i < tmp.size(); i++) {
                m_vArticulations.push_back(getElementDataInt("id", tmp[i]));
            }
        }

        tmp = getElements("viewpoint", rectString);
        if (tmp.size() > 0) {
            tmp = getElements("id", tmp[0]);
            for(unsigned i = 0; i < tmp.size(); i++) {
                m_vViewPoints.push_back(getElementDataInt("id", tmp[i]));
            }
        }

        vector <string> tmp2;

        tmp = getElements("annopoints", rectString);
        if (tmp.size() > 0) {
            tmp = getElements("point", tmp[0]);
            for(unsigned i = 0; i < tmp.size(); i++) {
                AnnoPoint p;

                tmp2 = getElements("id", tmp[i]);
                if (tmp2.size() > 0) {
                    p.id = getElementDataInt("id", tmp2[0]);
                }

                tmp2 = getElements("x", tmp[i]);
                if (tmp2.size() > 0) {
                    p.x = getElementDataInt("x", tmp2[0]);
                }

                tmp2 = getElements("y", tmp[i]);
                if (tmp2.size() > 0) {
                    p.y = getElementDataInt("y", tmp2[0]);
                }

                tmp2 = getElements("is_visible", tmp[i]);
                if (tmp2.size() > 0) {
                    p.is_visible = getElementDataInt("is_visible", tmp2[0]);
                }

                //       if (!(p.id > 0 && p.x > 0 && p.y > 0))
                //         cout << p.id << " " << p.x << " " << p.y << endl;

                //      assert(p.id >= 0 && p.x >= 0 && p.y >= 0);
                assert(p.id >= 0);

                m_vAnnoPoints.push_back(p);
            }
        }

        tmp = getElements("objpos", rectString);
        if (tmp.size() > 0) {
            tmp2 = getElements("x", tmp[0]);
            if (tmp2.size() > 0) {
                m_nObjPosX = getElementDataInt("x", tmp2[0]);
            }

            tmp2 = getElements("y", tmp[0]);
            if (tmp2.size() > 0) {
                m_nObjPosY = getElementDataInt("y", tmp2[0]);
            }
        }

        tmp = getElements("object_id", rectString);
        if (tmp.size() > 0) {
            m_nObjectId = getElementDataInt("object_id", tmp[0]);
        }

        tmp = getElements("motion_phase", rectString);
        if (tmp.size() > 0) {
            m_dMotionPhase = getElementDataFloat("motion_phase", tmp[0]);
        }

        //printXML();
    }

    void AnnoRect::parseIDL(const string &rectString) {
        string::size_type start = 1, end;
        end = rectString.find(",", start);
        //m_x1 = atoi(rectString.substr(start, end-start).c_str());
        m_x1 = atof(rectString.substr(start, end - start).c_str());
        //cout << m_x1 << endl;

        start = end + 1;
        end = rectString.find(",", start);
        //m_y1 = atoi(rectString.substr(start, end-start).c_str());
        m_y1 = atof(rectString.substr(start, end - start).c_str());
        //cout << m_y1 << endl;

        start = end + 1;
        end = rectString.find(",", start);
        //m_x2 = atoi(rectString.substr(start, end-start).c_str());
        m_x2 = atof(rectString.substr(start, end - start).c_str());
        //cout << m_x2 << endl;

        start = end + 1;
        end = rectString.find("):", start);
        //m_y2 = atoi(rectString.substr(start, end-start).c_str());
        m_y2 = atof(rectString.substr(start, end - start).c_str());
        //cout << m_y2 << endl;

        start = end + 2;
        end = rectString.find("/", start);
        m_dScore = atof(rectString.substr(start, end - start).c_str());
        //cout << m_dScore << endl;

        if (end == string::npos) {
            m_nSilhouetteID = -1;
        } else {
            start = end + 1;
            m_nSilhouetteID = atoi(rectString.substr(start, string::npos).c_str());
        }
        //cout << m_nSilhouetteID << endl;

        //printXML();
    }

    void AnnoRect::writeXML(ofstream &out) const {
        out << "    <annorect>\n";
        out << "        <x1>" << m_x1 << "</x1>\n";
        out << "        <y1>" << m_y1 << "</y1>\n";
        out << "        <x2>" << m_x2 << "</x2>\n";
        out << "        <y2>" << m_y2 << "</y2>\n";

        if (m_x3 >= 0) {
            out << "        <x3>" << m_x3 << "</x3>\n";
            out << "        <y3>" << m_y3 << "</y3>\n";
            out << "        <x4>" << m_x4 << "</x4>\n";
            out << "        <y4>" << m_y4 << "</y4>\n";
        }

        if (m_dScore != -1) {
            out << "        <score>" << m_dScore << "</score>\n";
        }

        if (m_nSilhouetteID != -1) {
            out << "        <silhouette>\n";
            out << "            <id>" << m_nSilhouetteID << "</id>\n";
            out << "        </silhouette>\n";
        }
        if (m_vArticulations.size() > 0) {
            out << "        <articulation>\n";
            vector<int>::const_iterator it;
            for(it = m_vArticulations.begin(); it != m_vArticulations.end(); it++) {
                out << "            <id>" << *it << "</id>\n";
            }
            out << "        </articulation>\n";
        }
        if (m_vViewPoints.size() > 0) {
            out << "        <viewpoint>\n";
            vector<int>::const_iterator it;
            for(it = m_vViewPoints.begin(); it != m_vViewPoints.end(); it++) {
                out << "            <id>" << *it << "</id>\n";
            }
            out << "        </viewpoint>\n";
        }

        if (m_nTrackID >= 0) {
            out << "        <track_id>" << m_nTrackID << "</track_id>\n";
        }

        if (m_vAnnoPoints.size() > 0) {
            out << "        <annopoints>\n";
            vector<AnnoPoint>::const_iterator it;
            for (it = m_vAnnoPoints.begin(); it != m_vAnnoPoints.end(); it++) {
                out << "          <point>\n";
                out << "          <id>" << (*it).id << "</id>\n";
                out << "          <x>" << (*it).x << "</x>\n";
                out << "          <y>" << (*it).y << "</y>\n";

                if ((*it).is_visible != -1) {
                    out << "          <is_visible>" << (*it).is_visible << "</is_visible>\n";
                }

                out << "          </point>\n";
            }
            out << "        </annopoints>\n";
        }

        if (m_nObjPosX > 0 && m_nObjPosY > 0) {
            out << "        <objpos>\n";
            out << "        <x>" << m_nObjPosX << "</x>\n";
            out << "        <y>" << m_nObjPosY << "</y>\n";
            out << "        </objpos>\n";
        }

        if (m_nObjectId >= 0) {
            out << "        <object_id>" << m_nObjectId << "</object_id>\n";
        }

        if (m_dMotionPhase >= 0) {
            out << "        <motion_phase>" << m_dMotionPhase << "</motion_phase>\n";
        }

        out << "    </annorect>\n";
    }


    void AnnoRect::printXML() const {
        cout << "    <annorect>\n";
        cout << "        <x1>" << m_x1 << "</x1>\n";
        cout << "        <y1>" << m_y1 << "</y1>\n";
        cout << "        <x2>" << m_x2 << "</x2>\n";
        cout << "        <y2>" << m_y2 << "</y2>\n";
        if (m_dScore != -1) {
            cout << "        <score>" << m_dScore << "</score>\n";
        }
        if (m_nSilhouetteID != -1) {
            cout << "        <silhouette>\n";
            cout << "            <id>" << m_nSilhouetteID << "</id>\n";
            cout << "        </silhouette>\n";
        }
        if (m_vArticulations.size() > 0) {
            cout << "        <articulation>\n";
            vector<int>::const_iterator it;
            for(it = m_vArticulations.begin(); it != m_vArticulations.end(); it++) {
                cout << "            <id>" << *it << "</id>\n";
            }
            cout << "        </articulation>\n";
        }
        if (m_vViewPoints.size() > 0) {
            cout << "        <viewpoint>\n";
            vector<int>::const_iterator it;
            for(it = m_vViewPoints.begin(); it != m_vViewPoints.end(); it++) {
                cout << "            <id>" << *it << "</id>\n";
            }
            cout << "        </viewpoint>\n";
        }

        if (m_nObjectId >= 0) {
            cout << "        <object_id>" << m_nObjectId << "</objpos>\n";
        }

        if (m_dMotionPhase >= 0) {
            cout << "        <motion_phase>" << m_dMotionPhase << "</motion_phase>\n";
        }

        cout << "    </annorect>\n";
    }

    void AnnoRect::writeIDL(ofstream &out) const {
        if (m_nSilhouetteID == -1) {
            out << "(" << m_x1 << ", " << m_y1 << ", " << m_x2 << ", " << m_y2 << "):" << m_dScore;
        } else {
            out << "(" << m_x1 << ", " << m_y1 << ", " << m_x2 << ", " << m_y2 << "):" << m_dScore << "/" << m_nSilhouetteID;
        }
    }

    void AnnoRect::printIDL() const {
        if (m_nSilhouetteID == -1) {
            cout << "(" << m_x1 << ", " << m_y1 << ", " << m_x2 << ", " << m_y2 << "):" << m_dScore;
        } else {
            cout << "(" << m_x1 << ", " << m_y1 << ", " << m_x2 << ", " << m_y2 << "):" << m_dScore << "/" << m_nSilhouetteID;
        }
    }

    void AnnoRect::sortCoords() {
        int tmp;
        if (m_x1 > m_x2) {
            tmp = m_x1;
            m_x1 = m_x2;
            m_x2 = tmp;
        }
        if (m_y1 > m_y2) {
            tmp = m_y1;
            m_y1 = m_y2;
            m_y2 = tmp;
        }
    }

    double AnnoRect::compCover( const AnnoRect &other ) const {
        AnnoRect r1 = *this;
        AnnoRect r2 = other;
        r1.sortCoords();
        r2.sortCoords();

        int nWidth  = r1.x2() - r1.x1();
        int nHeight = r1.y2() - r1.y1();
        int iWidth  = max(0, min(max(0, r2.x2() - r1.x1()), nWidth ) - max(0, r2.x1() - r1.x1()));
        int iHeight = max(0, min(max(0, r2.y2() - r1.y1()), nHeight) - max(0, r2.y1() - r1.y1()));
        return ((double)iWidth * (double)iHeight) / ((double)nWidth * (double)nHeight);
    }

    double AnnoRect::compRelDist( const AnnoRect &other, float dAspectRatio, FixDimType eFixObjDim ) const {
        double dWidth, dHeight;

        switch( eFixObjDim ) {
            case FIX_OBJWIDTH:
                dWidth  = m_x2 - m_x1;
                dHeight = dWidth / dAspectRatio;
                break;

            case FIX_OBJHEIGHT:
                dHeight = m_y2 - m_y1;
                dWidth  = dHeight * dAspectRatio;
                break;

            default:
                cerr << "Error in ImgDescrList::compRelDist(): "
                     << "Unknown type for parameter ('which obj dimension to fix?'): "
                     << eFixObjDim << "!" << endl;
                return -1.0;
        }

        double xdist = (double)(m_x1 + m_x2 - other.x1() - other.x2()) / dWidth;
        double ydist = (double)(m_y1 + m_y2 - other.y1() - other.y2()) / dHeight;
        return sqrt(xdist * xdist + ydist * ydist);
    }


    double AnnoRect::compRelDist( const AnnoRect &other ) const {
        double dWidth  = m_x2 - m_x1;
        double dHeight = m_y2 - m_y1;
        double xdist   = (double)(m_x1 + m_x2 - other.x1() - other.x2()) / dWidth;
        double ydist   = (double)(m_y1 + m_y2 - other.y1() - other.y2()) / dHeight;
        return sqrt(xdist * xdist + ydist * ydist);
    }


    // bool AnnoRect::isMatching( const AnnoRect& other, double dTDist, double dTCover, double dTOverlap, float dAspectRatio, FixDimType eFixObjDim )
    // {
    //   double dWidth, dHeight;
    //
    //   switch( eFixObjDim ) {
    //   case FIX_OBJWIDTH:
    //     dWidth  = m_x2 - m_x1;
    //     dHeight = dWidth / dAspectRatio;
    //     break;
    //
    //   case FIX_OBJHEIGHT:
    //     dHeight = m_y2 - m_y1;
    //     dWidth  = dHeight * dAspectRatio;
    //     break;
    //
    //   default:
    //     cerr << "Error in ImgDescrList::compRelDist(): "
    //          << "Unknown type for parameter ('which obj dimension to fix?'): "
    //          << eFixObjDim << "!" << endl;
    //     return -1.0;
    //   }
    //
    //   double xdist = (double)(m_x1+m_x2-other.x1()-other.x2()) / dWidth;
    //   double ydist = (double)(m_y1+m_y2-other.y1()-other.y2()) / dHeight;
    //   return sqrt(xdist*xdist + ydist*ydist);
    // }


    bool AnnoRect::isMatching( const AnnoRect &other, double dTDist, double dTCover, double dTOverlap) const {
        return ( (compRelDist(other) <= dTDist) &&
                 (compCover(other) >= dTCover) &&
                 (other.compCover(*this) >= dTOverlap) );
    }


} //end namespace


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
