#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkIdFilter.h>
#include <vtkNew.h>
#include <vtkPartitionedDataSet.h>
#include <vtkPartitionedDataSetCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkXMLPartitionedDataSetCollectionWriter.h>

int main(int, char*[])
{
  // vtkCompositeDataPipeline* exec = vtkCompositeDataPipeline::New();
  // vtkAlgorithm::SetDefaultExecutivePrototype(exec);
  // exec->Delete();
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

  vtkNew<vtkCompositeDataPipeline> cdp;
  vtkNew<vtkIdFilter> idF;
  idF->SetExecutive(cdp);
  idF->SetInputData(pdc);
  idF->SetPointIds(true);
  idF->FieldDataOn();

  idF->Update();

  auto out = idF->GetOutputDataObject(0);
  std::cout << "ID Filter Executive: " << idF->GetExecutive()->GetClassName() << std::endl;
  if (idF->GetOutputDataObject(0))
  {
    std::cout << "Output Class: " << idF->GetOutputDataObject(0)->GetClassName() << std::endl;
  }
  else
  {
    std::cout << "No luck" << std::endl;
  }

  // vtkNew<vtkXMLPartitionedDataSetCollectionWriter> w;
  // w->SetFileName("Input.vtpc");
  // w->SetInputData(pdc);
  // w->Write();
  // w->SetFileName("Output.vtpc");
  // w->SetInputData(idF);
  // w->Write();

  return EXIT_SUCCESS;
}
