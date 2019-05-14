// Ray.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Ray.h"
#include "Camera.h"
#include "Box.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
int nx = 200;
int ny = 200;
int ns = 100;
std::default_random_engine e;
std::uniform_real_distribution<float> Rand(0, 1);


inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

float GetFloat()
{
	return  0.0f;
}

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

	float ttt1 = 80.0f / GetFloat();
	float ttt2 = 70.0f / GetFloat();

	if (ttt1 > ttt2)
	{
		bool bRet = true;
	}



	//qsort()

	//return 0;


    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RAY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAY));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RAY);
	wcex.lpszMenuName = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
      800, 500, nx, ny, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

Vec3 Color(const Ray& R, Hitable *Shape,int depth);
void Print(HDC hdc);

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		/*
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
		*/
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...

			Print(hdc);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//利用随机的方法得到单位圆内的点。
Vec3 RandomInUnitSphere()
{
	Vec3 p;
	do
	{
		p = 2.0f * Vec3(Rand(e), Rand(e), Rand(e)) - Vec3(1.0f, 1.0f, 1.0f);
	} while (p.SquaredLength() > 1.0f);

	return p;
}

float Rand01()
{
	return Rand(e);
}

Vec3 Reflect(const Vec3& In, const Vec3& N)
{
	return In - 2 * Vec3::Dot(In, N) * N;
}

void random_scene(SphereContainer& SC) {
	int n = 500;

	SC.Add(new Sphere(Vec3(0, -1000, 0), 1000, new FLambertian(new FCheckerTexture(new FConstantTexture(Vec3{ 0.2f,0.3f,0.1f }), new FConstantTexture{ Vec3{0.9f,0.9f,0.9f} }))));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = Rand01();
			Vec3 center(a + 0.9*Rand01(), 0.2, b + 0.9*Rand01());
			if ((center - Vec3(4, 0.2, 0)).Length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					SC.Add(new Sphere(center, 0.2, new FLambertian(new FConstantTexture{ Vec3(Rand01()*Rand01(), Rand01()*Rand01(), Rand01()*Rand01()) })));
				}
				else if (choose_mat < 0.95) { // metal
					SC.Add(new Sphere(center, 0.2,
						new FMetal(Vec3(0.5*(1 + Rand01()), 0.5*(1 + Rand01()), 0.5*(1 + Rand01())), 0.5*Rand01())));
				}
				else {  // glass
					SC.Add(new Sphere(center, 0.2, new FDielectric(1.5)));
				}
			}
		}
	}

	SC.Add(new Sphere(Vec3(0, 1, 0), 1.0, new FDielectric(1.5)));
	SC.Add(new Sphere(Vec3(-4, 1, 0), 1.0, new FLambertian(new FConstantTexture{ Vec3(0.4, 0.2, 0.1) })));
	SC.Add(new Sphere(Vec3(4, 1, 0), 1.0, new FMetal(Vec3(0.7, 0.6, 0.5), 0.0)));

}
void RandomLight(SphereContainer& SC)
{
	SC.Add(new Sphere{ Vec3{0.0f ,-1000.0f ,0.0f},1000.0f,new FLambertian{
		new FCheckerTexture{new FConstantTexture(Vec3{ 0.2f,0.3f,0.1f }), new FConstantTexture{ Vec3{0.9f,0.9f,0.9f} }} } });

	SC.Add(new Sphere{ Vec3{0.0f ,2.0f ,0.0f},2.0f,new FLambertian{
	new FCheckerTexture{new FConstantTexture(Vec3{ 0.2f,0.3f,0.1f }), new FConstantTexture{ Vec3{0.9f,0.9f,0.9f} }} } });

	//SC.Add(new Sphere{ Vec3{ 0.0f,7.0f,0.0f },2.0f,new FLambertian{ new FConstantTexture{Vec3{1.0f,1.0f,1.0f}} } });

	SC.Add(new XYRect{ 3,5,1,3,-2,new FDiffuseLight{new FConstantTexture{Vec3{1.0f,1.0f,1.0f}}} });
}

void RandomShape(SphereContainer& SC)
{
	int ni, nj, nn;
	unsigned char *texData = stbi_load("earthmap.jpg", &ni, &nj, &nn, 0);
	SC.Add(new Sphere{ Vec3{ 0.0f,0.0f,-1.0f }, 0.5f ,new FLambertian{ new FImageTexture{texData,ni,nj}} });
    SC.Add(new Sphere{ Vec3{ 0.0f,-100.5f,-1.0f }, 100.0f ,new FLambertian{  new FConstantTexture{ Vec3{0.8f,0.8f,0.0f} } } });
	SC.Add(new Sphere{ Vec3{ 1.0f,0.0f,-1.0f }, 0.5f ,new FMetal     { Vec3{0.8f,0.6f,0.2f} , 0.0f } });
	SC.Add(new Sphere{ Vec3{-1.0f,-0.0f,-1.0f}, 0.5f ,new FDielectric { 1.5f } });
	SC.Add(new Sphere{ Vec3{-1.0f,-0.0f,-1.0f}, -0.4f ,new FDielectric { 1.5f } });    //电介质球体的一个有趣且简单的技巧是要注意，如果使用负半径，几何体不受影响但表面法线指向内部，因此它可以用作气泡来制作空心玻璃球体。
}

void cornell_box(SphereContainer& SC) {

	FMaterial *red = new FLambertian(new FConstantTexture(Vec3(0.65, 0.05, 0.05)));
	FMaterial *white = new FLambertian(new FConstantTexture(Vec3(0.73, 0.73, 0.73)));
	FMaterial *green = new FLambertian(new FConstantTexture(Vec3(0.12, 0.45, 0.15)));
	FMaterial *light = new FDiffuseLight(new FConstantTexture(Vec3(15, 15, 15)));
	SC.Add( new FlipNormal(new YZRect(0, 555, 0, 555, 555, green)));
	SC.Add(new YZRect(0, 555, 0, 555, 0, red));
	SC.Add(new XZRect(213, 343, 227, 332, 554, light));
	SC.Add(new FlipNormal(new XZRect(0, 555, 0, 555, 555, white)));
	SC.Add(new XZRect(0, 555, 0, 555, 0, white));
	SC.Add(new FlipNormal(new XYRect(0, 555, 0, 555, 555, white)));
	SC.Add(new FBox{ Vec3{130.0f,0.0f,65.0f},Vec3{295.0f,165.0f,230.0f},white });
	SC.Add(new FBox{ Vec3{265.0f,0.0f,295.0f},Vec3{430.0f,330.0f,460.0f},white });
}

void Print(HDC hdc)
{
	SphereContainer SC;
	//random_scene(SC);
	//RandomLight(SC);
	cornell_box(SC);

	//char szDebug[100] = { 0 };

	//FCamera Camera(Vec3(13.0f, 2.0f, 3.0f),Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,1.0f,0.0f),
	//	20.0f,static_cast<float>(nx)/static_cast<float>(ny));

	//FCamera Camera(Vec3(7.0f, 0.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),
	//	80.0f, static_cast<float>(nx) / static_cast<float>(ny));

	Vec3 lookfrom(278, 278, -800);
	Vec3 lookat(278, 278, 0);

	FCamera Camera(lookfrom, lookat, Vec3(0.0f, 1.0f, 0.0f),
		40.0f, static_cast<float>(nx) / static_cast<float>(ny));


	for (int j = 0; j < ny; ++j)
	{
		for (int i = 0; i < nx; ++i)
		{
			Vec3 C;

			//利用随机性，进行采样，然后求平均
			for (int s = 0; s < ns; ++s)
			{
				float fi = static_cast<float>(i) + Rand(e);
				float fj = static_cast<float>(j) + Rand(e);

				float U = (fi) / static_cast<float>(nx);
				float V = (fj) / static_cast<float>(ny);
				//sprintf_s(szDebug, "%f -- %f    %f -- %f \n", fi, fj, U,V);
				//OutputDebugStringA(szDebug);

				//从摄像机的感应平面发出的射线。
				Ray R = Camera.GetRay(U, V);

				//进行着色计算
				C += Color(R, &SC, 0);
			}
			C /= static_cast<float>(ns);

			SetPixel(hdc, i, j, C.ToCOLORREF());
		}
	}
}

Vec3 Color(const Ray& R,Hitable *Shape, int depth)
{
	HitRecord HR;

	//第一步：得到这根光线与某个shape的最近相交点，并返回相交点信息
	if (Shape->Hit(R, FLT_MAX, 0.001f, HR))
	{
		Ray Scattered;
		Vec3 Attenuation;

		//第二步：得到相交点对象的材质，然后根据材质，
		//        1、计算光线散射情况：计算光线经过相交点如何传输（漫反射，反射等等），得到处理后的光线，灯光材质不散射光线。
		//        2、计算向外辐射能量（目前只有灯光材质才向外辐射能量）。

		Vec3 Emit = HR.Material->Emitted(HR.U, HR.V, HR.P);
		if (depth < 50 && HR.Material->Scatter(R, HR, Attenuation, Scattered))
		{
			//第三步：递归处理这个新的光线。

			//返回 Emit01 + Albedo01 * ( Emit02 + Albedo02 * ( Emit03 + Albedo03 ...
			//1、光线经过3个漫反射材质没有到达灯光材质：Emit(0,0,0) + Albedo01 * ( Emit(0,0,0) + Albedo02 * ( Emit(0,0,0) +  Albedo03 * 背景(0,0,0) ) ) )，值为（0,0,0），茫茫黑暗。
			//                                              漫反射01                   漫反射02                    漫反射03              光线射线空中
			//
			return Emit + Attenuation * Color(Scattered, Shape, depth + 1);            
		}
		else
		{
			return Emit;
		}
	}
	else
	{
		
		return Vec3{ 0.0f,0.0f,0.0f };

		//根据（R.Direction().y）混合蓝色和白色
		Vec3 UnitDir = Vec3::UnitVector(R.Direction());        // -1 <= UnitDir.y <= 1
		float y = 0.5f * (UnitDir.Y() + 1.0f);                 // 转换到 0 <= y <= 1
		return (1.0f - y)*Vec3 { 1.0f, 1.0f, 1.0f } +y * Vec3{ 0.5f,0.7f,1.0f };         //混合蓝色和白色
	}
}


/*
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

*/
