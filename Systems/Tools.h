#ifndef _TOOLS
#define _TOOLS

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;

namespace Nautilus
{
	const double PI = 3.14159265;
	const Scalar RED = Scalar(0,0,255);
	const Scalar GREEN = Scalar(0,255,0);
	const Scalar BLUE = Scalar(255,0,0);
	const Scalar YELLOW = Scalar(0,255,255);
	const Scalar PURPLE = Scalar(255,0,255);
	const Scalar TEAL = Scalar(255,255,0);
	const Scalar BLACK = Scalar(0,0,0);
	const Scalar WHITE = Scalar(255,255,255);
	class Tools
	{
		public:
			static void displayContours(Mat &image, vector<vector<Point> > &contours, vector<Vec4i> &hierarchy, const Scalar color)
			{
				/*
					Displays contours onto an image with a specified color.
				*/
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					drawContours(image,contours,i,color,2,8,hierarchy,0,Point());
					drawContours(image,contours,i,color,2,8,hierarchy,0,Point());	
				}
			}
			static void displayCutLines(Mat &image, const int left, const int right, const int top, const int bottom, const Scalar color)
			{
				/*
					This function corresponds with cut() which takes an image and cuts out pixels by making them into a different color, usually black.
					You can specify how many pixels from the left,right,top, and bottom you wish to cut out, and you can specify which color
					you would like to turn them into (usually black or white). To demonstrate what we are cutting, I created this function
					which will place the lines onto an image so that we can view what exactly is being cut.
				*/
				int height = image.rows;
				int width = image.cols;
				line(image,Point(0,top),Point(width,top),color,1);
				line(image,Point(left,0),Point(left,height),color,1);
				line(image,Point(width - right,0),Point(width -	right,height),color,1);
				line(image,Point(0,height - bottom),Point(width,height - bottom),color,1);
			}
			static void displayCirclesOnImage(Mat &image,vector<Vec3f> circles, const Scalar color)
			{
				/*
					Using an object that represents circles, this function will place those circles onto
					the image with the specified color.
				*/
				for(int i = 0; i < (int)circles.size(); i++) //print out any circles that were found in the frame
				{
					circle(image, Point(cvRound(circles[i][0]),cvRound(circles[i][1])),3,color,1,8,0);
					circle(image, Point(cvRound(circles[i][0]),cvRound(circles[i][1])),cvRound(circles[i][2]),color,5,8,0);
				}
			}
			static void displayTimeOnImage(Mat &image, TimingSystem &timer, const Scalar color)
			{
				/*
					Places a timestamp at the topleft of the image using a TimingSystem and the color.
				*/
				String time = timer.getTimeStampHalf();
				putText(image,time,Point(15,15),1,1,color,2);
			}
			static void displayContoursDegrees(Mat &image, vector<vector<Point> > &contours, const Scalar &color1, const Scalar &color2)
			{
				/*
					Given a list of contours, it will display them all on an image specified.
					This function draws a line from the top left of the screen to the contour
					and uses Text to display it's degree position.
				*/
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					Point midPoint = getMidPoint(contours[i]);
					double xLocation = midPoint.x;
					double degrees = 180 - ((180 * xLocation) / image.cols);
					std::ostringstream output;
					output << degrees;
					line(image,Point(image.cols/2,image.rows),midPoint,color1);
					putText(image,output.str(),midPoint,1,1,color2);
				}
			}
			static void displayContoursMidPoints(Mat &image, vector<vector<Point> > &contours, const Scalar &color, int rad, int thick)
			{
				/*
					Cycles through a list of contours and places a small circle at the midpoint for each contour.
				*/
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					Point midPoint = getMidPoint(contours.at(i));
					circle(image,midPoint,2,color,rad,thick);
				}
			}
			static bool isValidPoint(const Point thePoint)
			{
				/*
					Returns true if the x and y of a point are non negative.
				*/
				return (thePoint.x < 0 || thePoint.y < 0);
			}

			static Point getMidPoint(vector<Point> &singleContour)
			{
				/*
					Estimates the midpoint of a contour, perhaps a better method would be
					to create the minimum fitting rectangle around the contour and then
					find the midpoint of that, keep that into consideration.
				*/
				int totalXCoords = 0;
				int totalYCoords = 0;

				for(unsigned int i = 0; i < singleContour.size(); i++)
				{
					totalXCoords+=singleContour[i].x;
					totalYCoords+=singleContour[i].y;
				}
				Point midPoint(totalXCoords/singleContour.size(),totalYCoords/singleContour.size());
				return midPoint;
			}
			static Point getMidPointDegrees(vector<Point> &singleContour, const int imageWidth, const int imageHeight)
			{
				/*
					Converts a singlecontour into a midpoint based on degrees from 0 to 180 in both the vertical
					and horizontal directions.
				*/
				Point midPoint = getMidPoint(singleContour);
				Point midPointDegrees;
				midPointDegrees.x = (int)getPointDegreesX(midPoint,imageWidth);
				midPointDegrees.y = (int)getPointDegreesY(midPoint,imageHeight);
				return midPointDegrees;
			}
			static Point getPointToDegrees(const Point &singlePoint, const int imageWidth, const int imageHeight)
			{
				/*
					Converts a point of pixels into a point of degrees based on the height and width
					of the camera images.
				*/
				Point pointInDegrees;
				pointInDegrees.x = (int)getPointDegreesX(singlePoint,imageWidth);
				pointInDegrees.y = (int)getPointDegreesY(singlePoint,imageHeight);
				return pointInDegrees;
			}
			static double getPointDegreesX(const Point &middlePoint, const int imageWidth)
			{
				/*
					Takes a Point and converts it into degrees based on the capture's width and height.
					returns the X degrees.
				*/
				double xLocation = middlePoint.x;
				double degrees = (180 * xLocation) / imageWidth;
				return degrees;
			}
			static double getPointDegreesY(const Point &middlePoint, const int imageHeight)
			{
				/*
					Takes a Point and converts it into degrees based on the capture's width and height.
					returns the Y degrees.
				*/
				double yLocation = middlePoint.y;
				//double degrees = 180 - ((180 * xLocation) / regularImage.rows);
				double degrees = (180 * yLocation) / imageHeight;
				return degrees;
			}
			static double degreesToPixelsX(const double degrees, const int imageWidth)
			{
				/*
					Returns a degree from 0-180 into it's pixel values
				*/
				return ((degrees * imageWidth) / 180);
			}
			static double degreesToPixelsY(const double degrees, const int imageHeight)
			{
				/*
					Returns a degree from 0-180 into it's pixel values
				*/
				return ((degrees * imageHeight) / 180);
			}
			static vector<vector<Point> > getContoursAtPoint(const vector<vector<Point> > &contours, const Point &p)
			{
				/*
					Iterates through contours and searches each of the contour's points checking if it is contained by
					the Point p. This function uses PointPolygonTest which determines if a Point lies inside of a contour.
					PointPolygonTest returns -1 if it was not contained, 0 if it lies on the edge, and 1 of it is contained by the
					contour.
				*/
				vector<vector<Point> > containedConts; //The contours which p lies in.
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					
					if(pointPolygonTest(contours[i],p,false) > -1)
						containedConts.push_back(contours[i]);
				}
				return containedConts;
			}

			static void removeContoursArea(vector<vector<Point> > &contours, const int area, const bool removeIfGreaterThan)
			{
				/*
					Removes contours based on their total pixel area.
					area specifies the minimum or maximum area for removing a specific contour.
					If removeIfGreaterThan == true then it will remove all contours that have an area greater than param area.
					If !removeIfGreaterThan then it will remove all contours with a smaller area than param area.
				*/
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					if( (removeIfGreaterThan && contourArea(contours[i]) > area ) 
						|| (!removeIfGreaterThan && contourArea(contours[i]) < area) )
					{
						contours.erase(contours.begin() + i);
						i--;
					}
				}
			}
			static void removeContoursHWRatio(vector<vector<Point> > &contours, const double HWRatio, const bool removeIfGreaterThan)
			{
				/*
					removeContourHWRatio Removes contours based on their Height-to-Width ratio. By averaging the
					X and Y points for each pixel in a contour we can calculate the averagedXPoints and
					averagedYPoints.
					If removeIfGreaterThan == true then it will remove all the contours that have a Height-ToWidth Ratio
					that is greater than HWRatio. Otherwise it will remove all the contours that have a smaller HWRatio.
				*/
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					double hwr = getHWRatio(contours[i]);
					if(removeIfGreaterThan && hwr > HWRatio)
					{
						contours.erase(contours.begin() + i);
						i--;
					}
					else if(!removeIfGreaterThan && hwr < HWRatio)
					{
						contours.erase(contours.begin() + i);
						i--;
					}
				}
			}
			static void removeContoursAboveLine(vector<vector<Point> > &contours, const int lineHeight, const bool useMidPoint = true)
			{
				/*
					This function cycles through all of the contours and removes them if they are above a certain pixel @lineHeight.
					If useMidPoint then the function will only test the values based on the midpoints of each contour.
					However if !useMidPoint then the function will test every single point for every contour, and if any of the points are
					above the threshold then the entire contour will be removed.
				*/
				if(useMidPoint)
				{
					for(unsigned int i = 0; i < contours.size(); i++)
					{
						Point contourMidPoint = getMidPoint(contours[i]);
						if(contourMidPoint.y < lineHeight)
						{
							contours.erase(contours.begin() + i);
							i--;
						}
					}
				}
				else
				{
					// Test every single point for each contour
					for(unsigned int i = 0; i < contours.size(); i++)
					{
						vector<Point> cont = contours[i];
						for(unsigned int j = 0; j < cont.size(); j++)
						{
							Point p = cont[j];
							if(p.y < lineHeight)
							{
								contours.erase(contours.begin() + i);
								i--;
								break;
							}
						}
					}
				}
			}
			static void removeContoursNearSky(vector<vector<Point> > &contours, Mat &image)
			{
				/*
					Attempts to detect if a contour is a mountain or at least somewhere near
					the top of the screen. This function attempts to avoid any actual coot contours, while
					still removing contours such as mountains or trees.
					It works by finding above a contours highest point and taking an average of all the pixels surrounding
					that high point. Using the pixel average we can determine that the contour is near the sky by looking
					at its hue. If the hue is greater than a certain amount then it is most likely at the base of the sky
					in the image.
					YBOOST shifts the average pixel area upward so that we are looking slightly above the contour.
				*/
				const int RADIUS = 9;
				const int MINHUE = 50;
				const int YBOOST = -4;
				for(unsigned int i = 0; i < contours.size(); i++)
				{
					//Get the highest point
					vector<Point> cont = contours.at(i);
					Tools::organizeByY(cont);
					Point highest = cont[cont.size() - 1];

					//Attempt to go a bit higher than the highest point just so we can get a more accurate reading
					if(Tools::isValidPoint(Point(highest.x,highest.y + YBOOST)))
						highest.y+=YBOOST;

					//Average the points and then check to remove the contour based on hue
					Scalar aver = Tools::averagePixelsNearby(image,highest,RADIUS,true);
					if(aver[0] > MINHUE)
					{
						contours.erase(contours.begin() + i);
						i--;
					}
				}
			}

			static Mat cutImage(Mat &image, const int imageWidth, const int imageHeight, const int left, const int right, const int top, const int bottom, const Scalar cutColor)
			{
				/*
					Similar to cropImage except it does not cause the image to zoom.
					This function creates 4 rectangles and fills them in to get rid of the specified areas.
					Scalar cutColor(0,0,0);
				*/
				rectangle(image,Point(0,0),Point(left,imageHeight),cutColor,-1,8);
				rectangle(image,Point(0,0),Point(imageWidth,top),cutColor,-1,8);
				rectangle(image,Point(imageWidth,imageHeight),Point(imageWidth - right,0),cutColor,-1,8);
				rectangle(image,Point(imageWidth,imageHeight),Point(0,imageHeight - bottom),cutColor,-1,8);
				return image;
			}

			static vector<Point> organizeByY(vector<Point> singleContour)
			{
				/*
					This function will take a single contour, and organize it's Y pixels so that the pixels closest
					to 0 come first (The pixels at the top of the screen).
				*/
				bool keepGoing = true;
				while(keepGoing)
				{
					keepGoing = false;
					for(unsigned int i = 0; i < singleContour.size() - 1; i++)
					{
						if(singleContour[i].y > singleContour[i+1].y)
						{
							keepGoing = true;
							Point tempPoint = singleContour[i];
							singleContour[i] = singleContour[i+1];
							singleContour[i+1] = tempPoint;
						}
					}
				}
				return singleContour;
			}
			static vector<Point> organizeByX(vector<Point> singleContour)
			{
				/*
					This function will take a single contour, and organize it's Y pixels so that the pixels closest
					to 0 come first (The pixels at the top of the screen).
				*/
				bool keepGoing = true;
				while(keepGoing)
				{
					keepGoing = false;
					for(unsigned int i = 0; i < singleContour.size() - 1; i++)
					{
						if(singleContour[i].x > singleContour[i+1].x)
						{
							keepGoing = true;
							Point tempPoint = singleContour[i];
							singleContour[i] = singleContour[i+1];
							singleContour[i+1] = tempPoint;
						}
					}
				}
				return singleContour;
			}
			static vector<vector<Point> > organizeByX(vector<vector<Point> > contours)
			{
				/*
					Takes an object of contours and sorts them based on their midpoints.
					The organization is from the left side of the screen to the right side
					(lowest X values to highest X Values)
				*/
				bool keepGoing = true;
				while(keepGoing)
				{
					keepGoing = false;
					int size = contours.size();
					if(size > 0)
					{
						for(int i = 0; i < (size - 1); i++)
						{
							Point firstMidPoint = getMidPoint(contours[i]);
							Point secondMidPoint = getMidPoint(contours[i+1]);
							if(firstMidPoint.x > secondMidPoint.x)
							{
								keepGoing = true;
								vector<Point> tempContour;
								tempContour = contours[i];
								contours[i] = contours[i+1];
								contours[i+1] = tempContour;
							}
						}
					}
				}
				return contours;
			}
			static vector<vector<Point> > organizeByY(vector<vector<Point> > contours)
			{
				/*
					Using a set of contours, it will organize each individual contour based on it's midpoint
					so that the contours with the lowest Y come first.
				*/
				bool keepGoing = true;
				while(keepGoing)
				{
					keepGoing = false;
					for(unsigned int i = 0; i < contours.size() - 1; i++)
					{
						Point midPoint1 = getMidPoint(contours[i]);
						Point midPoint2 = getMidPoint(contours[i+1]);
						if(midPoint1.y > midPoint2.y)
						{
							keepGoing = true;
							vector<Point> tempContour = contours[i];
							contours[i] = contours[i+1];
							contours[i+1] = tempContour;
						}
					}
				}
				return contours;
			}
			static vector<vector<Point> > organizeBySize(vector<vector<Point> > contours) //Organized by smallest to biggest
			{
				/*
					Uses contourArea to organize a set of contours so that the smallest contours come before the biggest.
				*/
				bool sort = true;
				while(sort)
				{
					sort = false;
					for(unsigned int i = 0; i < contours.size() - 1; i++)
					{
						double contour1Size = contourArea(contours[i]);
						double contour2Size = contourArea(contours[i+1]);
						if(contour1Size > contour2Size)
						{
							sort = true;
							vector<Point> tempContour = contours[i];
							contours[i] = contours[i+1];
							contours[i+1] = tempContour;
						}
					}
				}
				return contours;
			}

			static vector<Point> getLargestContour(vector<vector<Point> > &contours)
			{
				/*
					Finds the largest contour in a group of contours.
				*/
				vector<Point> largestContour = contours[0];
				for(unsigned int i = 1; i < contours.size(); i++)
				{
					if(contourArea(contours[i]) > contourArea(largestContour))
						largestContour = contours[i];
				}
				return largestContour;
			}
			static double getHWRatio(vector<Point> &singleContour)
			{
				/*
					Returns the Height/Width ratio for a contour.
				*/
				Rect r = boundingRect(singleContour);
				return static_cast<double>(r.height)/static_cast<double>(r.width);
			}
			static vector<double> getHeightAndWidth(vector<Point> &singleContour)
			{
				/*
					Returns a vector that contains both the height and width of a contour.
				*/
				Rect r = boundingRect(singleContour);
				vector<double> vec;
				vec.push_back(static_cast<double>(r.height));
				vec.push_back(static_cast<double>(r.width));
				return vec;
			}
			static vector<double> getHeightAndWidthDegrees(vector<Point> &singleContour, const int windowWidth, const int windowHeight)
			{
				/*
					Converts a singleContour into a height and width vector based on degrees.
					Utilizes getHeightAndWidth1 as the height and width function.
				*/
				vector<double> heightAndWidthVector = getHeightAndWidth(singleContour);
				vector<double> degreesVector;
				double heightDeg = ((180 * (double)heightAndWidthVector[0]) / windowHeight);
				double widthDeg = ((180 * (double)heightAndWidthVector[1]) / windowWidth);
				degreesVector.push_back(heightDeg);
				degreesVector.push_back(widthDeg);
				return degreesVector;
			}
			static double getRectangularAreaDegrees(vector<Point> &singleContour, const int windowWidth, const int windowHeight)
			{
				/*
					Tries to calculate how much area the single contour occupies, similar to contourArea.
				*/
				vector<double> degreesVector = getHeightAndWidthDegrees(singleContour, windowWidth, windowHeight);
				double degreeArea = degreesVector[0] * degreesVector[1];
				return degreeArea;		
			}
			
			static Scalar averagePixelsNearby(Mat &image, const Point &loc, const int radius, bool useHsv)
			{
				/* 
					Calculates a circular average of hsv colors that surround a specific point.
					This function creates a bounded square around where the circle would be located.
					It then uses the distance squared formula to determine if the pixel is inside the circle.
					useHSV returns the function in Hue Sat Val form.
				*/
				Point rectBound1 = Point(loc.x - radius, loc.y - radius);
				Point rectBound2 = Point(loc.x + radius, loc.y + radius);
				double b = 0, g = 0, r = 0;
				double count = 0;
				for(int i = rectBound1.x; i < rectBound2.x; i++)
				{
					for(int j = rectBound1.y; j < rectBound2.y; j++)
					{
						Point tempPoint = Point(i,j);
						if(isValidPoint(tempPoint))
						{
							double dx = loc.x - tempPoint.x;
							double dy = loc.y - tempPoint.y;
							double distSquared = dx * dx + dy * dy;
							if(distSquared <= (radius * radius))
							{
								Scalar scal = Scalar(image.at<Vec3b>(tempPoint));
								b+=scal[0];
								g+=scal[1];
								r+=scal[2];
								count++;
							}
						}
					}
				}
				b/=count;
				g/=count;
				r/=count;
				if(useHsv)
					return Tools::bgrToHsv(Scalar((int)b,(int)g,(int)r));
				return Scalar((int)b,(int)g,(int)r);
			}

			static string getAsciiValueString(const string targetString)
			{
				/*
					Converts each character in a string to its ascii characters.
					An example output would be something like: "897532513286" where every 2 digits
					could represent a specific character.
				*/
				string holderString = "";
				for(unsigned int i = 0; i < targetString.length(); i++)
				{
					holderString+=(int)targetString.at(i);
				}
				return holderString;
			}
			static Scalar hsvToBgr(Scalar hsvScalar)
			{
				/*
					This function will convert a HSV Scalar into a BGR Scalar. 
				*/
				int i;
				float h = (float)hsvScalar[0];
				float s = (float)hsvScalar[1];
				float v = (float)hsvScalar[2];
				float r = 0, g = 0, b = 0;
				float f, p, q, t;

				//Change the hue scale to 360
				h *= 2;
				s /= 255;
				v /= 255;

				if( s == 0 ) {
					// achromatic (grey)
					r = g = b = v;
					return Scalar((int)b,(int)g,(int)r);
				}

				h /= 60;			// sector 0 to 5
				i = (int)floor( h );
				f = h - i;			// factorial part of h
				p = v * ( 1 - s );
				q = v * ( 1 - s * f );
				t = v * ( 1 - s * ( 1 - f ) );

				switch( i ) {
					case 0:
						r = v;
						g = t;
						b = p;
						break;
					case 1:
						r = q;
						g = v;
						b = p;
						break;
					case 2:
						r = p;
						g = v;
						b = t;
						break;
					case 3:
						r = p;
						g = q;
						b = v;
						break;
					case 4:
						r = t;
						g = p;
						b = v;
						break;
					default:		// case 5:
						r = v;
						g = p;
						b = q;
						break;
				}
				//Rescale them back up
				b*=255;
				g*=255;
				r*=255;
				//Add any adjustments to keep them align with opencv
				b+=(float)0.35;
				g+=(float)0.35;
				return Scalar((int)b,(int)g,(int)r);
			}
			static Scalar bgrToHsv(Scalar bgrScalar)
			{
				/*
					Using a Scalar in BGR (blue, green, red) this function will return another
					Scalar that is in HSV format.
				*/
				double h,s,v;
				double r = bgrScalar[2];
				double g = bgrScalar[1];
				double b = bgrScalar[0];
				double rgbMin, rgbMax;

				rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
				rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

				v = rgbMax;
				if (v == 0)
				{
					h = 0;
					s = 0;
					return Scalar((int)h,(int)s,(int)v);
				}

				s = 255 * long(rgbMax - rgbMin) / v;
				if (s == 0)
				{
					h = 0;
					return Scalar((int)h,(int)s,(int)v);
				}

				if (rgbMax == r)
					h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
				else if (rgbMax == g)
					h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
				else
					h = 171 + 43 * (r - g) / (rgbMax - rgbMin);

				h = (int)(h + 256) % 256;
				//Rescale it to 180 with a slight round to make it more opencv accurate
				h = (((double)h/255) * 180); 
				h += 0.50;
				s += 0.35;

				return Scalar((int)h,(int)s,(int)v);
			}
			static bool doesPathExist(string &path)
			{
				/*
					Checks to see if a file already exists.
				*/
				ifstream file(path.c_str());
				return file.is_open();
			}
			static string addDuplicationToPath(string &path)
			{
				/*
					If a path already exists then we want to add a (#) to
					make sure that the previous file isn't going to be overwrote.
					For example:
						Nathan//Documents//homework.txt
						would be turned into
						Nathan//Documents//homework (1).txt
						if it previously existed.
				 */
				string temp = path;
				int i = 1;
				while(true)
				{
					ifstream ifile(temp.c_str());
					if(!ifile)
						break;

					ostringstream ost2;
					ost2 << path <<  "(" << i << ")";
					temp = ost2.str();

					i++;

					//Just incase we attempt to go too far
					if(i > 20)
						return path;
				}
				return temp;
			}
			static vector<string> split(const string &s, char delim)
			{
				/*
				Function: split
				Author: Nathan Braun-Krafft
				Description: Splits a string up based on a certain character.
					All the elements that are broken apart by delim will be added
					into a vector which will be returned.
				Inputs: A string to be split, the delimiter character that will be splitting the string.
				Outputs: A vector representing all of the split elements of the string.
				*/
				vector<string> vec;
				string temp = "";
				for(unsigned int i = 0; i < s.length(); i++)
				{
					char ch = s.at(i);
					if(ch != delim)
					{
						temp+=ch;
					}
					//If the char equals delimiter and we have values to add to the vector, we shall add them
					else if(ch == delim && temp != "") 
					{
						vec.push_back(temp);
						temp = "";
					}
				}

				//If there are still leftovers we need to add them to the vector.
				if(temp != "")
					vec.push_back(temp);

				return vec;
			}
			static void replaceAll(string &s, const string &oldstr, const string  &newstr)
			{
				/*
				Function: replaceAll
				Author: Nathan Braun-Krafft
				Description: Searches through the string s locating any character sequences that match
					oldstr, each of these sequences are replaced with newstr.
				Inputs: A string that will have its characters replaced,
					a sequence that will be replaced, a char sequence that will replace the old sequence.
				Outputs: None
				*/
				int foundLoc = 0;
				while((foundLoc = s.find(oldstr,foundLoc)) != string::npos)
				{
					s.replace(foundLoc,oldstr.length(),newstr);
					foundLoc+=newstr.length(); //Make sure not to double check something that has already been replaced.
				}
			}
};
}

#endif
