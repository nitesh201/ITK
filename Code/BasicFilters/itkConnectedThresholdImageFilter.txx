/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConnectedThresholdImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConnectedThresholdImageFilter_txx_
#define __itkConnectedThresholdImageFilter_txx_

#include "itkConnectedThresholdImageFilter.h"
#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"
#include "itkBinaryThresholdImageFunction.h"
#include "itkFloodFilledImageFunctionConditionalIterator.h"
#include "itkImageRegionIterator.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
ConnectedThresholdImageFilter<TInputImage, TOutputImage>
::ConnectedThresholdImageFilter()
{
  m_Lower = NumericTraits<InputImagePixelType>::NonpositiveMin();
  m_Upper = NumericTraits<InputImagePixelType>::max();
  m_Seed = IndexType::ZeroIndex;
  m_ReplaceValue = NumericTraits<OutputImagePixelType>::One;
}


/**
 * Standard PrintSelf method.
 */
template <class TInputImage, class TOutputImage>
void
ConnectedThresholdImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Upper: " << m_Upper << std::endl;
  os << indent << "Lower: " << m_Lower << std::endl;
}

template <class TInputImage, class TOutputImage>
void 
ConnectedThresholdImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() && this->GetOutput() )
    {
    this->GetInput()->SetRequestedRegionToLargestPossibleRegion();
    }
}


template <class TInputImage, class TOutputImage>
void 
ConnectedThresholdImageFilter<TInputImage,TOutputImage>
::GenerateData()
{
  InputImagePointer inputImage = this->GetInput();
  OutputImagePointer outputImage = this->GetOutput();

  // Zero the output
  outputImage->SetBufferedRegion( outputImage->GetRequestedRegion() );
  outputImage->Allocate();
  outputImage->FillBuffer ( NumericTraits<OutputImagePixelType>::Zero );
  
  typedef BinaryThresholdImageFunction<InputImageType> FunctionType;
  typedef FloodFilledImageFunctionConditionalIterator<OutputImageType, FunctionType> IteratorType;

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputImage ( inputImage );
  function->ThresholdBetween ( m_Lower, m_Upper );
  IteratorType it = IteratorType ( outputImage, function, m_Seed );

//   while( !it.IsAtEnd())
//     {
//     // std::cout << "Iterating" << std::endl << it.GetIndex() << std::endl;
//     it.Set(NumericTraits<OutputImagePixelType>::One);
//     ++it;
//     }

  typedef ImageRegionIterator<OutputImageType> RegionIteratorType;
  RegionIteratorType rit = RegionIteratorType ( outputImage, outputImage->GetRequestedRegion() );
  while( !rit.IsAtEnd())
    {
    // std::cout << "Iterating" << std::endl << it.GetIndex() << std::endl;
    rit.Set(m_ReplaceValue);
    ++it;
    }
  
}


} // end namespace itk

#endif
