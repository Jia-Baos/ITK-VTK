#include <iostream>
#include <string>

#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkPNGImageIOFactory.h>   // PNG
#include <itkFlipImageFilter.h>
#include <itkImageToVTKImageFilter.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
#include <vtkPNGReader.h>   // PNG
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>


int main(int argc, char* argv[])
{
    std::string image_path = "D:\\Code-VS\\picture\\match-data\\graffiti.png";

    // Init the environment
    itk::PNGImageIOFactory::RegisterOneFactory();

    // Define the PiexelType, Dimension and ImageType
    using PixelType = itk::RGBPixel<unsigned char>;
    constexpr unsigned int Dimension = 2;
    using ImageType = itk::Image<PixelType, Dimension>;

    // Get the image size
    ImageType::Pointer image = itk::ReadImage<ImageType>(image_path);
    ImageType::RegionType region = image->GetLargestPossibleRegion();
    ImageType::SizeType size = region.GetSize();
    std::cout << "cols: " << size[0] << "; " << "rows: " << size[1] << std::endl;

    //// Define the ReaderType and Pointer for Reader
    //using ReaderType = itk::ImageFileReader<ImageType>;
    //ReaderType::Pointer reader = ReaderType::New();
    //reader->SetFileName(image_path);

    // Define the FlipImageFilterType and flip the image
    using FlipImageFilterType = itk::FlipImageFilter<ImageType>;
    FlipImageFilterType::Pointer flipfilter = FlipImageFilterType::New();
    //flipfilter->SetInput(reader->GetOutput());
    flipfilter->SetInput(image);

    // Define the flip axes
    FlipImageFilterType::FlipAxesArrayType flipAxes;
    flipAxes[0] = false;
    flipAxes[1] = true;

    flipfilter->SetFlipAxes(flipAxes);
    flipfilter->Update();

    //Define the ImageToVTKImageFilter and capture the image
    using ConnectorType = itk::ImageToVTKImageFilter<ImageType>;
    ConnectorType::Pointer connector = ConnectorType::New();
    connector->SetInput(flipfilter->GetOutput());
    connector->Update();

    // Define the viewer and interator
    vtkImageViewer* viewer = vtkImageViewer::New();
    vtkRenderWindowInteractor* interator = vtkRenderWindowInteractor::New();
    viewer->SetInputData(connector->GetOutput());
    viewer->SetupInteractor(interator);
    //viewer->GetRenderWindow()->SetSize(484, 300);   //set window size
    viewer->SetColorLevel(128);     //set the level of window
    viewer->SetColorWindow(256);    //set window color
    viewer->Render();

    interator->Initialize();
    interator->Start();

    return EXIT_SUCCESS;
}
