#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Line {
    double m, c, a1, b1, c1, d1; // Slope and y-intercept

    Line(double a, double b, double c2, double d) : a1(a), b1(b), c1(c2), d1(d) {
        m = (double) (d - b) / (1.0*(c2 - a));
        c = b - (1.0*( m * a ));
    }
};
vector<Line> mergesort(vector<Line>& a);

bool doLinesIntersect(const Line& a, const Line& b) {
    // Checks if the lines are parallel (i.e., have the same slope)
    if (fabs(a.m - b.m)<1e-9) {
        // If they have the same slope, they are either parallel or coincident
        // Check if they are coincident by comparing y-intercepts
        if (fabs(a.c - b.c)<1e-9 ) {
            // If y-intercepts are the same, lines are coincident
            // and overlap over the entire range, so they intersect
            if((a.a1 > b.a1 && a.a1 < b.c1) || (b.a1 > a.a1 && b.a1 < a.c1))
                return true;
        }
        // If y-intercepts are different, lines are parallel and non-intersecting
        return false;
    }
    // Calculate intersection point
    double x_intersection = (double)(b.c - a.c) / (a.m - b.m);
    // Check if intersection point is within the range defined by both lines
    return ( x_intersection < min(a.c1, b.c1) && x_intersection > max(a.a1, b.a1) ) ;
}

// Find intersection x-coordinate of two lines
double intersection(const Line& a, const Line& b) {
    // condition if two lines are coincident
    if(fabs(a.m - b.m)<1e-9 && fabs(a.c - b.c)<1e-9)
        return min(a.c1,b.c1);
    
    return (b.c - a.c) / (a.m - b.m);
}



vector<Line> merge( vector<Line>& left, vector<Line>& right) {
   
    vector<Line> merged;
    size_t left_index = 0, right_index = 0;

    
    while (left_index < left.size() && right_index < right.size()) {
        // BDHIYA CASE ANALYSIS KR LIYA BRO

        Line l = left[left_index], r=right[right_index];
        if(fabs(l.a1-l.c1)<1e-9 )
        {
            left_index++;
            continue;
        }
         if(fabs(r.a1-r.c1)<1e-9 )
        {
            right_index++;
            continue;
        }

        bool dolinescross = doLinesIntersect(l,r); 

        if(left[left_index].a1 <= right[right_index].a1)  //left vale ka left x-coordinate km h
        {
            if(dolinescross){  
                if(fabs(l.m - r.m)<1e-9 && fabs(l.c - r.c)<1e-9)
                {
                    if(l.c1 <= r.c1)
                    {
                        merged.push_back(l);
                        left_index++;
                        right[right_index].a1 = l.c1;    
                        right[right_index].b1 = l.d1;      
                        continue;
                    }  // r beyond tk chl rhi h

                    else{  // l beyond tk chl rhi h
                        Line x(l.a1, l.b1, r.c1, r.d1);
                        merged.push_back(x);
                        right_index++;
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = r.d1;
                        continue;
                    }
                } // coincident lines

                double x_intersect = intersection(l,r);
                // OG intersection ke cases - 4cases , depending on left vala upr ya niche , and left ka c1 < / > right ka c1 
                
                if((l.a1 - r.a1)*(l.b1 - r.b1) > 0) // right vala initially upr h
                {
                    if(l.c1 <= r.c1)  // r beyond l chl rha h 
                    {
                        Line x(l.a1, l.b1, r.a1, l.m*1.0*r.a1 + l.c);
                        merged.push_back(x);
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, l.c1 , l.d1 );
                        merged.push_back(z);
                        left_index++;
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        continue;
                    }
                    else {  // l beyond r chl rha h
                        Line x(l.a1, l.b1, r.a1, l.m*1.0*r.a1 + l.c);
                     
                        merged.push_back(x);
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, r.c1 , l.m*1.0*r.c1 + l.c );
                        merged.push_back(z);
                        right_index++;
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        continue;
                    }
                } 

                else if(fabs(l.a1 - r.a1)<1e-6)    // equal start point of intersecting lines , be careful
                {
                   if(fabs(l.c1 - r.c1)<1e-6)    // equal start and end points 
                    {
                        if(l.b1 <= r.b1)  // pehle r bda y-coordinate
                        {
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, l.c1 , l.d1 );
                        merged.push_back(z); 
                        }
                        else{    // pehle l bda y-coordinate
                        Line y(l.a1, l.b1, x_intersect, l.m*1.0*x_intersect + l.c );
                        merged.push_back(y);
                        Line z( x_intersect, l.m*1.0*x_intersect + l.c, r.c1 , r.d1 );
                        merged.push_back(z); 
                        }
                        left_index++;
                        right_index++;
                        continue;
                    }

                    if(l.b1 <= r.b1)  // pehle r bda y-coordinate
                    {
                        if(l.c1 <= r.c1)  //  r extends beyond l
                        {
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, l.c1 , l.d1 );
                        merged.push_back(z); 
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        left_index++;
                        continue;
                        }
                        else{     // l extends beyond r
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, r.c1 , l.m*1.0*r.c1 + l.c );
                        merged.push_back(z); 
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        right_index++;
                        continue;
                        }
                    }
                    else{       // pehle l bda y-coordinate

                    if(l.c1 <= r.c1)  //  r extends beyond l
                        {
                        Line y(l.a1, l.b1, x_intersect, l.m*1.0*x_intersect + l.c );
                        merged.push_back(y);
                        Line z( x_intersect, l.m*1.0*x_intersect + l.c, l.c1 , r.m*1.0*l.c1 + r.c );
                        merged.push_back(z); 
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        left_index++;
                        continue;
                        }
                        else{     // l extends beyond r
                        Line y(l.a1, l.b1, x_intersect, l.m*1.0*x_intersect + l.c );
                        merged.push_back(y);
                        Line z( x_intersect, l.m*1.0*x_intersect + l.c, r.c1 , r.d1 );
                        merged.push_back(z); 
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        right_index++;
                        continue;
                        }
                    }
                }

                else{    // left half vala initially upr h
   
                    if(l.c1 <= r.c1)  // r beyond l chl rha h 
                    {
                      
                        Line y(l.a1, l.b1, x_intersect, l.m*1.0*x_intersect + l.c );
                        merged.push_back(y);
                        Line z( x_intersect, l.m*1.0*x_intersect + l.c, l.c1 , r.m*1.0*l.c1 + r.c );
                        merged.push_back(z);
                        left_index++;
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        continue;
                    }

                    else{  // l beyond r chl rha h
                        Line y(l.a1, l.b1, x_intersect, l.m*1.0*x_intersect + l.c );
                        merged.push_back(y);
                        Line z( x_intersect, l.m*1.0*x_intersect + l.c, r.c1 , r.d1 );
                        merged.push_back(z);
                        right_index++;
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        continue;
                    }
                }
            } // lines cross each other 
 

            else{ // lines dont cross - 4 cases , disjoint , contained , partially contained-2
 
                if(l.c1 <= r.a1)
                {
                    merged.push_back(l);
                    left_index++;
                    continue;
                }// disjoint lines

                if(r.c1 > l.c1)  // l is partailly contained , r extend ho rha
                {
                    double l_value_at_left_project = l.m*1.0*r.a1 + l.c ;
                    if( l_value_at_left_project <= r.b1 )
                    {
                        Line x(l.a1, l.b1, r.a1, l_value_at_left_project);
                        merged.push_back(x);
                        left_index++;
                        continue;
                    } // right vali  will overtake before finishing of left

                    else{   // right vali overtakes after left 
                        merged.push_back(l);
                        left_index++;
                        right[right_index].a1 = l.c1 ;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c ;
                        continue;
                    }   

                }    
                 
                
                else{   // r is partially contained , l beyond extend ho rha h 
                    double l_value_at_left_project = l.m*1.0*r.a1 + l.c ;
                        if( l_value_at_left_project <= r.b1 )
                    {
                        Line x(l.a1, l.b1, r.a1, l_value_at_left_project);
                        merged.push_back(x);
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        merged.push_back(r);
                        right_index++;
                        continue;
                    } // right vali about to overtake 

                    else{   // right vali always contained within
                        Line x(l.a1, l.b1, r.c1, l.m*1.0*r.c1 + l.c );
                        merged.push_back(x);
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        right_index++;
                        continue;
                    }   
                }
            }// lines dont cross - 4 cases , disjoint , contained , partially contained-2 

        }//left vale ka left x-coordinate km h
        
        

        else{ //right vale ka  left x-ccordinate  km h

            if(dolinescross){
                if(fabs(l.m - r.m)<1e-9 && fabs(l.c - r.c)<1e-9)  // coincident lines
                {
                    if(l.c1 <= r.c1)
                    {
                        Line x(r.a1, r.b1, l.c1, l.d1);
                        merged.push_back(x);
                         left_index++;
                         right[right_index].a1 = l.c1;    
                        right[right_index].b1 = l.d1;   
                        continue;
                    }  // r beyond l tk chl rhi h

                    else{  // l beyond r tk chl rhi h
                        
                        merged.push_back(r);
                        right_index++;
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = r.d1;
                        continue;
                    }
                } // coincident lines


                double x_intersect = intersection(l,r);
                // OG intersection ke cases - 4cases , depending on left vala upr ya niche , and left ka c1 < / > right ka c1 
                
                if((l.a1 - r.a1)*(l.b1 - r.b1) >= 0) // left subhalf initially upr h
                {
                    if(r.c1 <= l.c1)  // l beyond r chl rha h 
                    {
                        Line x(r.a1, r.b1, l.a1, r.m*1.0*l.a1 + r.c);
                        merged.push_back(x);
                        Line y(l.a1, l.b1, x_intersect, l.m*1.0*x_intersect + l.c );
                        merged.push_back(y);
                        Line z( x_intersect, l.m*1.0*x_intersect + l.c, r.c1 , r.d1 );
                        merged.push_back(z);
                        right_index++;
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        continue;
                    }

                    else{  // r beyond l chl rha h
                        Line x(r.a1, r.b1, l.a1, r.m*1.0*l.a1 + r.c);
                        merged.push_back(x);
                        Line y(l.a1, l.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, l.c1 , r.m*1.0*l.c1 + r.c );
                        merged.push_back(z);
                        left_index++;
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        continue;
                    }

                } 

                else{  // right subhalf is initially upr
 
                    if(r.c1 <= l.c1)  // l beyond r chl rha h 
                    {
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, r.c1 , l.m*1.0*r.c1 + l.c );
                        merged.push_back(z);
                        right_index++;
                        left[left_index].a1 = r.c1;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c;
                        continue;
                    }

                    else{  // r beyond l chl rha h
                        Line y(r.a1, r.b1, x_intersect, r.m*1.0*x_intersect + r.c );
                        merged.push_back(y);
                        Line z( x_intersect, r.m*1.0*x_intersect + r.c, l.c1 , l.d1 );
                        merged.push_back(z);
                        left_index++;
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        continue;
                    }
                }

            } // lines cross each other  
 


            else{ // lines don't cross each other 

                if(r.c1 <= l.a1)
                {
                    merged.push_back(r);
                    right_index++;
                    continue;
                }// disjoint lines

                if(l.c1 > r.c1)     // r is partailly contained , l extend ho rha
                {
                    double r_value_at_right_project = r.m*1.0*l.a1 + r.c ;
                    if( r_value_at_right_project <= l.b1 )
                    {
                        Line x(r.a1, r.b1, l.a1, r_value_at_right_project);
                        merged.push_back(x);
                        right_index++;
                        continue;
                    } // left one about overtake 

                    else{   // left overtakes oly after right 
                        merged.push_back(r);
                        right_index++;
                        left[left_index].a1 = r.c1 ;
                        left[left_index].b1 = l.m*1.0*r.c1 + l.c ;
                        continue;
                    }   

                }    
                 
                else{   // l is partially contained , r beyond extend ho rha h 

                    double r_value_at_right_project = r.m*1.0*l.a1 + r.c ;
                        if( r_value_at_right_project <= l.b1 )
                    {
                        Line x(r.a1, r.b1, l.a1, r_value_at_right_project);
                        merged.push_back(x);
                        merged.push_back(l);
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        left_index++;
                        continue;
                    } // left will overtake 

                    else{   // left one always contained
                        Line x(r.a1, r.b1, l.c1, r.m*1.0*l.c1 + r.c );
                        merged.push_back(x);
                        right[right_index].a1 = l.c1;
                        right[right_index].b1 = r.m*1.0*l.c1 + r.c;
                        left_index++;
                        continue;
                    }   
                }

            }// lines dont cross - 4 cases , disjoint , contained , partially contained-2 
        

        }

    }

    // Copy remaining elements from left and right vectors

     while (left_index < left.size()) {
        
        merged.push_back(left[left_index]);
        left_index++;
    }
    while (right_index < right.size()) {
        merged.push_back(right[right_index]);
        right_index++;
    }
    return merged;
}

vector<Line> mergesort(vector<Line>& lines) {
    if (lines.size() <= 1)
        return lines;

    // Divide the vector into two halves
     
    size_t mid = lines.size() / 2;
    vector<Line> left_half(lines.begin(), lines.begin() + mid);
    vector<Line> right_half(lines.begin() + mid, lines.end());
    vector<Line> left_visible , right_visible;
    // Recursively sort the two halves
    left_visible = mergesort(left_half);
    right_visible = mergesort(right_half);

         return merge(left_visible, right_visible);
}



int main() {
    int n;
    std::cin >> n; // Number of lines

    std::vector<Line> lines;
    for (int i = 0; i < n; ++i) {
        double a,b,c,d;
        cin>>a>>b>>c>>d;
        lines.emplace_back(a, b,c,d);
    }

    // Sort lines by left x-coordinates in descending order
    std::sort(lines.begin(), lines.end(), [](const Line& a, const Line& b) {
        return a.a1 < b.a1;
    });

    vector<Line> visibleSegments = mergesort(lines);


    double area = 0;
    for (int i = 0; i < visibleSegments.size(); ++i) {
        Line l = visibleSegments[i];
        area += 0.5 * (l.b1 + l.d1)*(l.c1 - l.a1);
    }
    cout<<(long)area<<endl;

    double length = 0;
    for (int i = 0; i < visibleSegments.size(); ++i) {
        Line l = visibleSegments[i];
        length += (l.c1 - l.a1);
    }
    cout<<(long)length<<endl;

    return 0;
}