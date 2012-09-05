#ifndef PROP_H
#define PROP_H

class CProp
{
public:
	CProp(int x=0,int y=0,int w=0,int h=0):_coordinateX(x),_coordinateY(y),_width(w),_height(h)
	{
		_oldX = x;
		_oldY = y;
	}
	~CProp(){}
	inline int getX() const
	{
		return _coordinateX;
	}
	inline void setX(const int x)
	{
		_coordinateX = x;
	}
	inline int getY() const
	{
		return _coordinateY;
	}
	inline void setY(const int y)
	{
		_coordinateY = y;
	}
	inline int getW() const
	{
		return _width;
	}
	inline void setW(const int w)
	{
		_width = w;
	}
	inline int getH() const
	{
		return _height;
	}
	inline void setH(const int h)
	{
		_height = h;
	}
	inline int getOldX() const
	{
		return _oldX;
	}
	inline void setOldX(const int ox)
	{
		_oldX = ox;
	}
	inline int getOldY() const
	{
		return _oldY;
	}
	inline void setOldY(const int oy)
	{
		_oldY = oy;
	}
	virtual void draw();
	virtual bool isCollide(const CProp& other);
private:
	int _coordinateX;
	int _coordinateY;
	int _oldX; // Last x coordinate
	int _oldY; // Last y coordinate
	int _width;
	int _height;
};

#endif