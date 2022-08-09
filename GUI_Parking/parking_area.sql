-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Jul 31, 2022 at 12:40 PM
-- Server version: 10.4.24-MariaDB
-- PHP Version: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `parking_area`
--

-- --------------------------------------------------------

--
-- Table structure for table `tb_log_peralatan`
--

CREATE TABLE `tb_log_peralatan` (
  `no` int(100) NOT NULL,
  `tanggal` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `status_device` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `tb_log_peralatan`
--

INSERT INTO `tb_log_peralatan` (`no`, `tanggal`, `status_device`) VALUES
(1, '2022-07-29 10:39:59', 'ONLINE'),
(2, '2022-07-29 10:40:28', 'OFFLINE'),
(3, '2022-07-30 17:21:05', 'ONLINE'),
(4, '2022-07-30 17:21:42', 'ONLINE'),
(5, '2022-07-30 17:21:48', 'ONLINE');

-- --------------------------------------------------------

--
-- Table structure for table `tb_log_transaksi`
--

CREATE TABLE `tb_log_transaksi` (
  `no` int(100) NOT NULL,
  `tanggal` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `uid` varchar(100) NOT NULL,
  `cabang` varchar(100) NOT NULL,
  `gerbang` varchar(100) NOT NULL,
  `gardu` varchar(100) NOT NULL,
  `saldo_awal` bigint(255) NOT NULL,
  `tarif` bigint(255) NOT NULL,
  `saldo_akhir` bigint(255) NOT NULL,
  `status_transaksi` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `tb_log_transaksi`
--

INSERT INTO `tb_log_transaksi` (`no`, `tanggal`, `uid`, `cabang`, `gerbang`, `gardu`, `saldo_awal`, `tarif`, `saldo_akhir`, `status_transaksi`) VALUES
(1, '2022-07-30 17:21:05', 'kamui', 'bali', 'barat', 'utara', 123456, 50000, 86141, 'Sukses'),
(2, '2022-07-30 17:21:42', 'kamui', 'bali', 'barat', 'utara', 123456, 50000, 86141, 'Sukses'),
(3, '2022-07-30 17:21:48', 'kamui', 'bali', 'barat', 'utara', 123456, 50000, 86141, 'Sukses');

-- --------------------------------------------------------

--
-- Table structure for table `tb_monitor`
--

CREATE TABLE `tb_monitor` (
  `uid` varchar(100) NOT NULL,
  `saldo_akhir` int(100) NOT NULL,
  `status_transaksi` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `tb_monitor`
--

INSERT INTO `tb_monitor` (`uid`, `saldo_akhir`, `status_transaksi`) VALUES
('FC86HAHA', 980022, 'Sukses');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `tb_log_peralatan`
--
ALTER TABLE `tb_log_peralatan`
  ADD PRIMARY KEY (`no`);

--
-- Indexes for table `tb_log_transaksi`
--
ALTER TABLE `tb_log_transaksi`
  ADD PRIMARY KEY (`no`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `tb_log_peralatan`
--
ALTER TABLE `tb_log_peralatan`
  MODIFY `no` int(100) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `tb_log_transaksi`
--
ALTER TABLE `tb_log_transaksi`
  MODIFY `no` int(100) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
