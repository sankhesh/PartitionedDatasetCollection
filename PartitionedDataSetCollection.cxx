#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPartitionedDataSetCollection.h>
#include <vtkPartitionedDataSet.h>
#include <vtkSphereSource.h>
#include <vtkIdFilter.h>
#include <vtkXMLPartitionedDataSetCollectionWriter.h>
#include <vtkCompositeDataPipeline.h>

int main(int, char* [])
{
  vtkCompositeDataPipeline* exec = vtkCompositeDataPipeline::New();
  vtkAlgorithm::SetDefaultExecutivePrototype(exec);
  exec->Delete();
  vtkNew<vtkPartitionedDataSetCollection> pdc;
  for (unsigned int part = 0; part < 5; ++part)
  {
    vtkNew<vtkPartitionedDataSet> pd;
    for (unsigned int cc = 0; cc <= part; ++cc)
    {
      vtkNew<vtkSphereSource> sphere;
      sphere->SetCenter(cc, part, 0);
      sphere->Update();

      vtkDataObject* poly = sphere->GetOutputDataObject(0);
      pd->SetPartition(cc, poly);
    }
    pdc->SetPartitionedDataSet(part, pd);
  }

  std::cout << "Input Class: " << pdc->GetClassName() << std::endl;

  // vtkNew<vtkCompositeDataPipeline> cdp;
  vtkNew<vtkIdFilter> idF;
  // idF->SetExecutive(cdp);
  idF->SetInputData(pdc->GetPartition(0, 0));
  idF->SetPointIds(true);
  idF->SetPointIdsArrayName("PID");
  idF->FieldDataOn();


  idF->Update();

  std::cout << "Output Class " << std::endl;
  if (idF->GetOutput())
  {
    std::cout << "Output Class: " << idF->GetOutput()->GetClassName() << std::endl;
  }
  else
  {
    std::cout << "No luck" << std::endl;
  }


  // vtkNew<vtkXMLPartitionedDataSetCollectionWriter> w;
  // w->SetFileName("Test.vtpc");
  // w->SetInputData(pdc);
  // w->Write();

  return EXIT_SUCCESS;
}
