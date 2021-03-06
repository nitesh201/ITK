/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkHoughTransform2DCirclesImageFilter_h
#define itkHoughTransform2DCirclesImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkEllipseSpatialObject.h"

namespace itk
{
/**
 * \class HoughTransform2DCirclesImageFilter
 * \brief Performs the Hough Transform to find circles in a 2D image.
 *
 * This filter derives from the base class ImageToImageFilter.
 * The input is an image, and all pixels above some threshold are those
 * we want to consider during the process.
 *
 * This filter produces two output:
 *   1) The accumulator array, which represents probability of centers.
 *   2) The array or radii, which has the radius value at each coordinate point.
 *
 * When the filter finds a "correct" point, it computes the gradient at this
 * point and draws a regular narrow-banded circle using the minimum and maximum
 * radius given by the user, and fills in the array of radii.
 * The SweepAngle value can be adjusted to improve the segmentation.
 *
 * The filter will detect ring-shaped objects in the image, but it also finds discs.
 * For a disc to be found, the intensity values within the disc must be higher than
 * the surrounding of the disc.
 *
 * TOutputPixelType is the pixel type of the accumulator image. An unsigned integer
 * type (like 'unsigned long') is usually the best choice for this pixel type.
 *
 * TRadiusPixelType is the pixel type of the radius image. A floating point type
 * is recommended, as the estimation of the radius involves floating point
 * calculations. Usually, 'double' is the best choice for this pixel type.
 *
 * By default, TRadiusPixelType = TOutputPixelType, in order to preserve backward
 * compatibility with ITK <= 4.12.2.
 *
 * \ingroup ImageFeatureExtraction
 *
 * \ingroup ITKImageFeature
 */

template< typename TInputPixelType, typename TOutputPixelType, typename TRadiusPixelType = TOutputPixelType  >
class ITK_TEMPLATE_EXPORT HoughTransform2DCirclesImageFilter:
  public ImageToImageFilter< Image< TInputPixelType, 2 >, Image< TOutputPixelType, 2 > >
{
public:

  /** Standard class type aliases. */
  using Self = HoughTransform2DCirclesImageFilter;
  using Superclass = ImageToImageFilter< Image< TInputPixelType, 2 >,
                              Image< TOutputPixelType, 2 > >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Input Image type alias. */
  using InputImageType = Image< TInputPixelType, 2 >;
  using InputImagePointer = typename InputImageType::Pointer;
  using InputImageConstPointer = typename InputImageType::ConstPointer;

  /** Output Image type alias. */
  using OutputImageType = Image< TOutputPixelType, 2 >;
  using OutputImagePointer = typename OutputImageType::Pointer;

  /** Radius Image type alias. */
  using RadiusImageType = Image< TRadiusPixelType, 2 >;
  using RadiusImagePointer = typename RadiusImageType::Pointer;

  /** Image index type alias. */
  using IndexType = typename InputImageType::IndexType;

  /** Image pixel value type alias. */
  using PixelType = typename InputImageType::PixelType;

  /** Typedef to describe the output image region type. */
  using OutputImageRegionType = typename InputImageType::RegionType;

  /** Circle type alias. */
  using CircleType = EllipseSpatialObject< 2 >;
  using CirclePointer = typename CircleType::Pointer;
  using CirclesListType = std::list< CirclePointer >;

  using CirclesListSizeType = typename CirclesListType::size_type;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HoughTransform2DCirclesImageFilter, ImageToImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Method for evaluating the implicit function over the image. */
  void GenerateData() override;

  /** Set both Minimum and Maximum radius values. */
  void SetRadius(double radius);

  /** Set the minimum radius value the filter should look for. */
  itkSetMacro(MinimumRadius, double);
  itkGetConstMacro(MinimumRadius, double);

  /** Set the maximum radius value the filter should look for. */
  itkSetMacro(MaximumRadius, double);
  itkGetConstMacro(MaximumRadius, double);

  /** Set the threshold above which the filter should consider
   * the point as a valid point. */
  itkSetMacro(Threshold, double);

  /** Get the threshold value. */
  itkGetConstMacro(Threshold, double);

  /** Get the radius image. */
  itkGetModifiableObjectMacro(RadiusImage, RadiusImageType);

  /** Set the scale of the derivative function (using DoG). */
  itkSetMacro(SigmaGradient, double);

  /** Get the scale value. */
  itkGetConstMacro(SigmaGradient, double);

  /** Get the list of circles. This recomputes the circles. */
  CirclesListType & GetCircles();

  /** Set/Get the number of circles to extract. */
  itkSetMacro(NumberOfCircles, CirclesListSizeType);
  itkGetConstMacro(NumberOfCircles, CirclesListSizeType);

  /** Set/Get the radius of the disc to remove from the accumulator
   * for each circle found. */
  itkSetMacro(DiscRadiusRatio, float);
  itkGetConstMacro(DiscRadiusRatio, float);

  /** Set/Get the variance of the Gaussian blurring for the accumulator. */
  itkSetMacro(Variance, float);
  itkGetConstMacro(Variance, float);

  /** Set/Get the sweep angle. */
  itkSetMacro(SweepAngle, float);
  itkGetConstMacro(SweepAngle, float);

#ifdef ITK_USE_CONCEPT_CHECKING
  // Begin concept checking
  itkConceptMacro( IntConvertibleToOutputCheck,
                   ( Concept::Convertible< int, TOutputPixelType > ) );
  itkConceptMacro( InputGreaterThanDoubleCheck,
                   ( Concept::GreaterThanComparable< PixelType, double > ) );
  itkConceptMacro( OutputPlusIntCheck,
                   ( Concept::AdditiveOperators< TOutputPixelType, int > ) );
  itkConceptMacro( OutputDividedByIntCheck,
                   ( Concept::DivisionOperators< TOutputPixelType, int > ) );
  // End concept checking
#endif

protected:

  HoughTransform2DCirclesImageFilter();
  ~HoughTransform2DCirclesImageFilter() override {}

  void PrintSelf(std::ostream & os, Indent indent) const override;

  /** HoughTransform2DCirclesImageFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() override;

  /** HoughTransform2DCirclesImageFilter's produces all the output.
   * Therefore, it must provide an implementation of
   * EnlargeOutputRequestedRegion.
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  void EnlargeOutputRequestedRegion( DataObject *itkNotUsed(output) ) override;

private:

  ITK_DISALLOW_COPY_AND_ASSIGN(HoughTransform2DCirclesImageFilter);

  float                 m_SweepAngle;
  double                m_MinimumRadius;
  double                m_MaximumRadius;
  double                m_Threshold;
  double                m_SigmaGradient;

  RadiusImagePointer    m_RadiusImage;
  CirclesListType       m_CirclesList;
  CirclesListSizeType   m_NumberOfCircles;
  float                 m_DiscRadiusRatio;
  float                 m_Variance;
  ModifiedTimeType      m_OldModifiedTime;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHoughTransform2DCirclesImageFilter.hxx"
#endif

#endif
