// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// Issue reported on LinkedIn

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

#include "../tools.hpp"

#include <memory>

namespace stress_size {

////////////////////////////////////////////////////////////////////////////////

struct state_info_t {};

using Config = hfsm2::Config::ContextT<const std::shared_ptr<state_info_t>&>;

using M = hfsm2::MachineT<Config>;

//------------------------------------------------------------------------------

//#pragma region FSM

#define S(s) struct s

// this type is 30,040 chars long..

using FSM =
	M::Root<
		S(S_start),
		S(S0),
		S(S1),
		M::Composite<
			S(exc_1),
			S(split_exc_1),/* The cross-road state */
			M::Composite<
				S(seq_1),
				S(S2),
				S(S3),
				S(S4),
				M::Composite<
					S(par_2), /* Submachine name */
					M::Orthogonal<
						S(regions_par_2), /* Submachine name */
						M::Composite<
							S(seq_2),
							S(seq_2_loop_check),
							S(S5)
						>,
						M::Composite<
							S(seq_3),
							M::Composite<
								S(seq_4),
								S(seq_4_loop_check),
								M::Composite<
									S(par_3), /* Submachine name */
									M::Orthogonal<
										S(regions_par_3), /* Submachine name */
										M::Composite<
											S(seq_5),
											S(S6)
										>,
										M::Composite<
											S(seq_6),
											S(S7),
											S(S8),
											S(S9)
										>
									>,
									S(join_par_3)
								>
							>,
							S(S10)
						>
					>,
					S(join_par_2)
				>,
				M::Composite<
					S(exc_4),
					S(split_exc_4),/* The cross-road state */
					M::Composite<
						S(seq_7),
						S(S11),
						S(S12),
						M::Composite<
							S(par_5), /* Submachine name */
							M::Orthogonal<
								S(regions_par_5), /* Submachine name */
								S(S13),
								S(S14)
							>,
							S(join_par_5)
						>
					>,
					M::Composite<
						S(seq_8),
						S(S15),
						S(S16),
						M::Composite<
							S(par_6), /* Submachine name */
							M::Orthogonal<
								S(regions_par_6), /* Submachine name */
								S(S17),
								S(S18)
							>,
							S(join_par_6)
						>
					>,
					S(join_exc_4)
				>
			>,
			M::Composite<
				S(seq_9),
				S(S19)
			>,
			S(join_exc_1)
		>,
		S(S20),
		M::Composite<
			S(exc_7),
			S(split_exc_7),/* The cross-road state */
			M::Composite<
				S(seq_10),
				S(S21),
				M::Composite<
					S(exc_8),
					S(split_exc_8),/* The cross-road state */
					M::Composite<
						S(seq_11),
						S(S22),
						S(S23),
						M::Composite<
							S(par_9), /* Submachine name */
							M::Orthogonal<
								S(regions_par_9), /* Submachine name */
								S(S24),
								S(S25)
							>,
							S(join_par_9)
						>
					>,
					M::Composite<
						S(seq_12),
						S(S26),
						S(S27),
						M::Composite<
							S(par_10), /* Submachine name */
							M::Orthogonal<
								S(regions_par_10), /* Submachine name */
								S(S28),
								S(S29)
							>,
							S(join_par_10)
						>
					>,
					S(join_exc_8)
				>,
				S(S30)
			>,
			M::Composite<
				S(seq_13),
				S(S31)
			>,
			S(join_exc_7)
		>,
		S(S32),
		S(S33),
		M::Composite<
			S(exc_11),
			S(split_exc_11),/* The cross-road state */
			M::Composite<
				S(seq_14),
				M::Composite<
					S(exc_12),
					S(split_exc_12),/* The cross-road state */
					M::Composite<
						S(seq_15),
						S(S34),
						S(S35),
						S(S36),
						S(S37),
						M::Composite<
							S(par_13), /* Submachine name */
							M::Orthogonal<
								S(regions_par_13), /* Submachine name */
								M::Composite<
									S(seq_16),
									M::Composite<
										S(seq_17),
										S(seq_17_loop_check),
										M::Composite<
											S(exc_14),
											S(split_exc_14),/* The cross-road state */
											M::Composite<
												S(seq_18),
												M::Composite<
													S(exc_15),
													S(split_exc_15),/* The cross-road state */
													M::Composite<
														S(seq_19),
														S(S38),
														S(S39)
													>,
													S(join_exc_15)
												>
											>,
											M::Composite<
												S(seq_20),
												M::Composite<
													S(exc_16),
													S(split_exc_16),/* The cross-road state */
													M::Composite<
														S(seq_21),
														M::Composite<
															S(exc_17),
															S(split_exc_17),/* The cross-road state */
															M::Composite<
																S(seq_22),
																S(S40)
															>,
															M::Composite<
																S(seq_23),
																S(S41),
																S(S42)
															>,
															S(join_exc_17)
														>
													>,
													M::Composite<
														S(seq_24),
														S(S43),
														S(S44)
													>,
													S(join_exc_16)
												>
											>,
											S(join_exc_14)
										>
									>,
									S(S45)
								>,
								M::Composite<
									S(seq_25),
									M::Composite<
										S(seq_26),
										S(seq_26_loop_check),
										M::Composite<
											S(par_18), /* Submachine name */
											M::Orthogonal<
												S(regions_par_18), /* Submachine name */
												M::Composite<
													S(seq_27),
													S(S46),
													S(S47),
													S(S48)
												>,
												M::Composite<
													S(seq_28),
													S(S49),
													S(S50),
													S(S51),
													S(S52),
													S(S53)
												>,
												M::Composite<
													S(seq_29),
													S(S54),
													S(S55),
													S(S56),
													S(S57),
													S(S58),
													S(S59)
												>,
												M::Composite<
													S(seq_30),
													S(S60),
													S(S61),
													S(S62),
													S(S63),
													S(S64),
													S(S65)
												>,
												M::Composite<
													S(seq_31),
													S(S66),
													S(S67),
													S(S68)
												>
											>,
											S(join_par_18)
										>
									>,
									S(S69)
								>
							>,
							S(join_par_13)
						>
					>,
					S(join_exc_12)
				>
			>,
			S(join_exc_11)
		>,
		S(S70),
		M::Composite<
			S(exc_19),
			S(split_exc_19),/* The cross-road state */
			M::Composite<
				S(seq_32),
				M::Composite<
					S(exc_20),
					S(split_exc_20),/* The cross-road state */
					M::Composite<
						S(seq_33),
						S(S71)
					>,
					M::Composite<
						S(seq_34),
						S(S72)
					>,
					S(join_exc_20)
				>
			>,
			S(join_exc_19)
		>,
		S(S73),
		M::Composite<
			S(exc_21),
			S(split_exc_21),/* The cross-road state */
			M::Composite<
				S(seq_35),
				M::Composite<
					S(exc_22),
					S(split_exc_22),/* The cross-road state */
					M::Composite<
						S(seq_36),
						M::Composite<
							S(exc_23),
							S(split_exc_23),/* The cross-road state */
							M::Composite<
								S(seq_37),
								S(S74),
								M::Composite<
									S(exc_24),
									S(split_exc_24),/* The cross-road state */
									M::Composite<
										S(seq_38),
										S(S75),
										S(S76),
										S(S77),
										M::Composite<
											S(par_25), /* Submachine name */
											M::Orthogonal<
												S(regions_par_25), /* Submachine name */
												S(S78),
												S(S79)
											>,
											S(join_par_25)
										>
									>,
									M::Composite<
										S(seq_39),
										S(S80),
										M::Composite<
											S(exc_26),
											S(split_exc_26),/* The cross-road state */
											M::Composite<
												S(seq_40),
												M::Composite<
													S(exc_27),
													S(split_exc_27),/* The cross-road state */
													M::Composite<
														S(seq_41),
														S(S81),
														S(S82)
													>,
													M::Composite<
														S(seq_42),
														S(S83)
													>,
													S(join_exc_27)
												>,
												S(S84)
											>,
											M::Composite<
												S(seq_43),
												M::Composite<
													S(exc_28),
													S(split_exc_28),/* The cross-road state */
													M::Composite<
														S(seq_44),
														S(S85),
														S(S86),
														S(S87),
														M::Composite<
															S(par_29), /* Submachine name */
															M::Orthogonal<
																S(regions_par_29), /* Submachine name */
																M::Composite<
																	S(seq_45),
																	S(S88),
																	S(S89)
																>,
																M::Composite<
																	S(seq_46),
																	S(S90),
																	S(S91)
																>
															>,
															S(join_par_29)
														>
													>,
													M::Composite<
														S(seq_47),
														S(S92),
														S(S93),
														M::Composite<
															S(par_30), /* Submachine name */
															M::Orthogonal<
																S(regions_par_30), /* Submachine name */
																M::Composite<
																	S(seq_48),
																	S(S94),
																	S(S95)
																>,
																M::Composite<
																	S(seq_49),
																	S(S96),
																	S(S97)
																>
															>,
															S(join_par_30)
														>
													>,
													S(join_exc_28)
												>
											>,
											S(join_exc_26)
										>
									>,
									S(join_exc_24)
								>,
								S(S98)
							>,
							M::Composite<
								S(seq_50),
								M::Composite<
									S(exc_31),
									S(split_exc_31),/* The cross-road state */
									M::Composite<
										S(seq_51),
										S(S99),
										S(S100),
										M::Composite<
											S(par_32), /* Submachine name */
											M::Orthogonal<
												S(regions_par_32), /* Submachine name */
												S(S101),
												S(S102)
											>,
											S(join_par_32)
										>
									>,
									M::Composite<
										S(seq_52),
										S(S103),
										S(S104),
										M::Composite<
											S(par_33), /* Submachine name */
											M::Orthogonal<
												S(regions_par_33), /* Submachine name */
												S(S105),
												S(S106)
											>,
											S(join_par_33)
										>
									>,
									S(join_exc_31)
								>
							>,
							S(join_exc_23)
						>
					>,
					M::Composite<
						S(seq_53),
						M::Composite<
							S(exc_34),
							S(split_exc_34),/* The cross-road state */
							M::Composite<
								S(seq_54),
								S(S107),
								M::Composite<
									S(exc_35),
									S(split_exc_35),/* The cross-road state */
									M::Composite<
										S(seq_55),
										S(S108),
										S(S109),
										S(S110),
										M::Composite<
											S(par_36), /* Submachine name */
											M::Orthogonal<
												S(regions_par_36), /* Submachine name */
												S(S111),
												S(S112)
											>,
											S(join_par_36)
										>
									>,
									M::Composite<
										S(seq_56),
										S(S113),
										M::Composite<
											S(exc_37),
											S(split_exc_37),/* The cross-road state */
											M::Composite<
												S(seq_57),
												M::Composite<
													S(exc_38),
													S(split_exc_38),/* The cross-road state */
													M::Composite<
														S(seq_58),
														S(S114),
														S(S115)
													>,
													M::Composite<
														S(seq_59),
														S(S116)
													>,
													S(join_exc_38)
												>,
												S(S117)
											>,
											M::Composite<
												S(seq_60),
												M::Composite<
													S(exc_39),
													S(split_exc_39),/* The cross-road state */
													M::Composite<
														S(seq_61),
														S(S118),
														S(S119),
														S(S120),
														M::Composite<
															S(par_40), /* Submachine name */
															M::Orthogonal<
																S(regions_par_40), /* Submachine name */
																M::Composite<
																	S(seq_62),
																	S(S121),
																	S(S122)
																>,
																M::Composite<
																	S(seq_63),
																	S(S123),
																	S(S124)
																>
															>,
															S(join_par_40)
														>
													>,
													M::Composite<
														S(seq_64),
														S(S125),
														S(S126),
														M::Composite<
															S(par_41), /* Submachine name */
															M::Orthogonal<
																S(regions_par_41), /* Submachine name */
																M::Composite<
																	S(seq_65),
																	S(S127),
																	S(S128)
																>,
																M::Composite<
																	S(seq_66),
																	S(S129),
																	S(S130)
																>
															>,
															S(join_par_41)
														>
													>,
													S(join_exc_39)
												>
											>,
											S(join_exc_37)
										>
									>,
									S(join_exc_35)
								>,
								S(S131)
							>,
							M::Composite<
								S(seq_67),
								M::Composite<
									S(exc_42),
									S(split_exc_42),/* The cross-road state */
									M::Composite<
										S(seq_68),
										S(S132),
										S(S133),
										M::Composite<
											S(par_43), /* Submachine name */
											M::Orthogonal<
												S(regions_par_43), /* Submachine name */
												S(S134),
												S(S135)
											>,
											S(join_par_43)
										>
									>,
									M::Composite<
										S(seq_69),
										S(S136),
										S(S137),
										M::Composite<
											S(par_44), /* Submachine name */
											M::Orthogonal<
												S(regions_par_44), /* Submachine name */
												S(S138),
												S(S139)
											>,
											S(join_par_44)
										>
									>,
									S(join_exc_42)
								>
							>,
							S(join_exc_34)
						>
					>,
					S(join_exc_22)
				>,
				M::Composite<
					S(exc_45),
					S(split_exc_45),/* The cross-road state */
					M::Composite<
						S(seq_70),
						M::Composite<
							S(exc_46),
							S(split_exc_46),/* The cross-road state */
							M::Composite<
								S(seq_71),
								S(S140),
								M::Composite<
									S(par_47), /* Submachine name */
									M::Orthogonal<
										S(regions_par_47), /* Submachine name */
										S(S141),
										S(S142)
									>,
									S(join_par_47)
								>
							>,
							M::Composite<
								S(seq_72),
								S(S143)
							>,
							S(join_exc_46)
						>
					>,
					S(join_exc_45)
				>
			>,
			S(join_exc_21)
		>,
		S(S144),
		M::Composite<
			S(exc_48),
			S(split_exc_48),/* The cross-road state */
			M::Composite<
				S(seq_73),
				S(S145)
			>,
			S(join_exc_48)
		>,
		M::Composite<
			S(exc_49),
			S(split_exc_49),/* The cross-road state */
			M::Composite<
				S(seq_74),
				S(S146),
				S(S147)
			>,
			S(join_exc_49)
		>,
		S(S148),
		S(S149),
		S(S150),
		M::Composite<
			S(exc_50),
			S(split_exc_50),/* The cross-road state */
			M::Composite<
				S(seq_75),
				M::Composite<
					S(exc_51),
					S(split_exc_51),/* The cross-road state */
					M::Composite<
						S(seq_76),
						S(S151)
					>,
					S(join_exc_51)
				>,
				S(S152)
			>,
			M::Composite<
				S(seq_77),
				M::Composite<
					S(exc_52),
					S(split_exc_52),/* The cross-road state */
					M::Composite<
						S(seq_78),
						S(S153)
					>,
					S(join_exc_52)
				>
			>,
			S(join_exc_50)
		>,
		S(S_end)
	>;

#undef S

//#pragma endregion

////////////////////////////////////////////////////////////////////////////////

//#pragma region States

struct S_start : FSM::State {};
struct S0 : FSM::State {};
struct S1 : FSM::State {};
struct exc_1 : FSM::State {};
struct split_exc_1 : FSM::State {};
struct seq_1 : FSM::State {};
struct S2 : FSM::State {};
struct S3 : FSM::State {};
struct S4 : FSM::State {};
struct par_2 : FSM::State {};
struct regions_par_2 : FSM::State {};
struct seq_2 : FSM::State {};
struct seq_2_loop_check : FSM::State {};
struct S5 : FSM::State {};
struct seq_3 : FSM::State {};
struct seq_4 : FSM::State {};
struct seq_4_loop_check : FSM::State {};
struct par_3 : FSM::State {};
struct regions_par_3 : FSM::State {};
struct seq_5 : FSM::State {};
struct S6 : FSM::State {};
struct seq_6 : FSM::State {};
struct S7 : FSM::State {};
struct S8 : FSM::State {};
struct S9 : FSM::State {};
struct join_par_3 : FSM::State {};
struct S10 : FSM::State {};
struct join_par_2 : FSM::State {};
struct exc_4 : FSM::State {};
struct split_exc_4 : FSM::State {};
struct seq_7 : FSM::State {};
struct S11 : FSM::State {};
struct S12 : FSM::State {};
struct par_5 : FSM::State {};
struct regions_par_5 : FSM::State {};
struct S13 : FSM::State {};
struct S14 : FSM::State {};
struct join_par_5 : FSM::State {};
struct seq_8 : FSM::State {};
struct S15 : FSM::State {};
struct S16 : FSM::State {};
struct par_6 : FSM::State {};
struct regions_par_6 : FSM::State {};
struct S17 : FSM::State {};
struct S18 : FSM::State {};
struct join_par_6 : FSM::State {};
struct join_exc_4 : FSM::State {};
struct seq_9 : FSM::State {};
struct S19 : FSM::State {};
struct join_exc_1 : FSM::State {};
struct S20 : FSM::State {};
struct exc_7 : FSM::State {};
struct split_exc_7 : FSM::State {};
struct seq_10 : FSM::State {};
struct S21 : FSM::State {};
struct exc_8 : FSM::State {};
struct split_exc_8 : FSM::State {};
struct seq_11 : FSM::State {};
struct S22 : FSM::State {};
struct S23 : FSM::State {};
struct par_9 : FSM::State {};
struct regions_par_9 : FSM::State {};
struct S24 : FSM::State {};
struct S25 : FSM::State {};
struct join_par_9 : FSM::State {};
struct seq_12 : FSM::State {};
struct S26 : FSM::State {};
struct S27 : FSM::State {};
struct par_10 : FSM::State {};
struct regions_par_10 : FSM::State {};
struct S28 : FSM::State {};
struct S29 : FSM::State {};
struct join_par_10 : FSM::State {};
struct join_exc_8 : FSM::State {};
struct S30 : FSM::State {};
struct seq_13 : FSM::State {};
struct S31 : FSM::State {};
struct join_exc_7 : FSM::State {};
struct S32 : FSM::State {};
struct S33 : FSM::State {};
struct exc_11 : FSM::State {};
struct split_exc_11 : FSM::State {};
struct seq_14 : FSM::State {};
struct exc_12 : FSM::State {};
struct split_exc_12 : FSM::State {};
struct seq_15 : FSM::State {};
struct S34 : FSM::State {};
struct S35 : FSM::State {};
struct S36 : FSM::State {};
struct S37 : FSM::State {};
struct par_13 : FSM::State {};
struct regions_par_13 : FSM::State {};
struct seq_16 : FSM::State {};
struct seq_17 : FSM::State {};
struct seq_17_loop_check : FSM::State {};
struct exc_14 : FSM::State {};
struct split_exc_14 : FSM::State {};
struct seq_18 : FSM::State {};
struct exc_15 : FSM::State {};
struct split_exc_15 : FSM::State {};
struct seq_19 : FSM::State {};
struct S38 : FSM::State {};
struct S39 : FSM::State {};
struct join_exc_15 : FSM::State {};
struct seq_20 : FSM::State {};
struct exc_16 : FSM::State {};
struct split_exc_16 : FSM::State {};
struct seq_21 : FSM::State {};
struct exc_17 : FSM::State {};
struct split_exc_17 : FSM::State {};
struct seq_22 : FSM::State {};
struct S40 : FSM::State {};
struct seq_23 : FSM::State {};
struct S41 : FSM::State {};
struct S42 : FSM::State {};
struct join_exc_17 : FSM::State {};
struct seq_24 : FSM::State {};
struct S43 : FSM::State {};
struct S44 : FSM::State {};
struct join_exc_16 : FSM::State {};
struct join_exc_14 : FSM::State {};
struct S45 : FSM::State {};
struct seq_25 : FSM::State {};
struct seq_26 : FSM::State {};
struct seq_26_loop_check : FSM::State {};
struct par_18 : FSM::State {};
struct regions_par_18 : FSM::State {};
struct seq_27 : FSM::State {};
struct S46 : FSM::State {};
struct S47 : FSM::State {};
struct S48 : FSM::State {};
struct seq_28 : FSM::State {};
struct S49 : FSM::State {};
struct S50 : FSM::State {};
struct S51 : FSM::State {};
struct S52 : FSM::State {};
struct S53 : FSM::State {};
struct seq_29 : FSM::State {};
struct S54 : FSM::State {};
struct S55 : FSM::State {};
struct S56 : FSM::State {};
struct S57 : FSM::State {};
struct S58 : FSM::State {};
struct S59 : FSM::State {};
struct seq_30 : FSM::State {};
struct S60 : FSM::State {};
struct S61 : FSM::State {};
struct S62 : FSM::State {};
struct S63 : FSM::State {};
struct S64 : FSM::State {};
struct S65 : FSM::State {};
struct seq_31 : FSM::State {};
struct S66 : FSM::State {};
struct S67 : FSM::State {};
struct S68 : FSM::State {};
struct join_par_18 : FSM::State {};
struct S69 : FSM::State {};
struct join_par_13 : FSM::State {};
struct join_exc_12 : FSM::State {};
struct join_exc_11 : FSM::State {};
struct S70 : FSM::State {};
struct exc_19 : FSM::State {};
struct split_exc_19 : FSM::State {};
struct seq_32 : FSM::State {};
struct exc_20 : FSM::State {};
struct split_exc_20 : FSM::State {};
struct seq_33 : FSM::State {};
struct S71 : FSM::State {};
struct seq_34 : FSM::State {};
struct S72 : FSM::State {};
struct join_exc_20 : FSM::State {};
struct join_exc_19 : FSM::State {};
struct S73 : FSM::State {};
struct exc_21 : FSM::State {};
struct split_exc_21 : FSM::State {};
struct seq_35 : FSM::State {};
struct exc_22 : FSM::State {};
struct split_exc_22 : FSM::State {};
struct seq_36 : FSM::State {};
struct exc_23 : FSM::State {};
struct split_exc_23 : FSM::State {};
struct seq_37 : FSM::State {};
struct S74 : FSM::State {};
struct exc_24 : FSM::State {};
struct split_exc_24 : FSM::State {};
struct seq_38 : FSM::State {};
struct S75 : FSM::State {};
struct S76 : FSM::State {};
struct S77 : FSM::State {};
struct par_25 : FSM::State {};
struct regions_par_25 : FSM::State {};
struct S78 : FSM::State {};
struct S79 : FSM::State {};
struct join_par_25 : FSM::State {};
struct seq_39 : FSM::State {};
struct S80 : FSM::State {};
struct exc_26 : FSM::State {};
struct split_exc_26 : FSM::State {};
struct seq_40 : FSM::State {};
struct exc_27 : FSM::State {};
struct split_exc_27 : FSM::State {};
struct seq_41 : FSM::State {};
struct S81 : FSM::State {};
struct S82 : FSM::State {};
struct seq_42 : FSM::State {};
struct S83 : FSM::State {};
struct join_exc_27 : FSM::State {};
struct S84 : FSM::State {};
struct seq_43 : FSM::State {};
struct exc_28 : FSM::State {};
struct split_exc_28 : FSM::State {};
struct seq_44 : FSM::State {};
struct S85 : FSM::State {};
struct S86 : FSM::State {};
struct S87 : FSM::State {};
struct par_29 : FSM::State {};
struct regions_par_29 : FSM::State {};
struct seq_45 : FSM::State {};
struct S88 : FSM::State {};
struct S89 : FSM::State {};
struct seq_46 : FSM::State {};
struct S90 : FSM::State {};
struct S91 : FSM::State {};
struct join_par_29 : FSM::State {};
struct seq_47 : FSM::State {};
struct S92 : FSM::State {};
struct S93 : FSM::State {};
struct par_30 : FSM::State {};
struct regions_par_30 : FSM::State {};
struct seq_48 : FSM::State {};
struct S94 : FSM::State {};
struct S95 : FSM::State {};
struct seq_49 : FSM::State {};
struct S96 : FSM::State {};
struct S97 : FSM::State {};
struct join_par_30 : FSM::State {};
struct join_exc_28 : FSM::State {};
struct join_exc_26 : FSM::State {};
struct join_exc_24 : FSM::State {};
struct S98 : FSM::State {};
struct seq_50 : FSM::State {};
struct exc_31 : FSM::State {};
struct split_exc_31 : FSM::State {};
struct seq_51 : FSM::State {};
struct S99 : FSM::State {};
struct S100 : FSM::State {};
struct par_32 : FSM::State {};
struct regions_par_32 : FSM::State {};
struct S101 : FSM::State {};
struct S102 : FSM::State {};
struct join_par_32 : FSM::State {};
struct seq_52 : FSM::State {};
struct S103 : FSM::State {};
struct S104 : FSM::State {};
struct par_33 : FSM::State {};
struct regions_par_33 : FSM::State {};
struct S105 : FSM::State {};
struct S106 : FSM::State {};
struct join_par_33 : FSM::State {};
struct join_exc_31 : FSM::State {};
struct join_exc_23 : FSM::State {};
struct seq_53 : FSM::State {};
struct exc_34 : FSM::State {};
struct split_exc_34 : FSM::State {};
struct seq_54 : FSM::State {};
struct S107 : FSM::State {};
struct exc_35 : FSM::State {};
struct split_exc_35 : FSM::State {};
struct seq_55 : FSM::State {};
struct S108 : FSM::State {};
struct S109 : FSM::State {};
struct S110 : FSM::State {};
struct par_36 : FSM::State {};
struct regions_par_36 : FSM::State {};
struct S111 : FSM::State {};
struct S112 : FSM::State {};
struct join_par_36 : FSM::State {};
struct seq_56 : FSM::State {};
struct S113 : FSM::State {};
struct exc_37 : FSM::State {};
struct split_exc_37 : FSM::State {};
struct seq_57 : FSM::State {};
struct exc_38 : FSM::State {};
struct split_exc_38 : FSM::State {};
struct seq_58 : FSM::State {};
struct S114 : FSM::State {};
struct S115 : FSM::State {};
struct seq_59 : FSM::State {};
struct S116 : FSM::State {};
struct join_exc_38 : FSM::State {};
struct S117 : FSM::State {};
struct seq_60 : FSM::State {};
struct exc_39 : FSM::State {};
struct split_exc_39 : FSM::State {};
struct seq_61 : FSM::State {};
struct S118 : FSM::State {};
struct S119 : FSM::State {};
struct S120 : FSM::State {};
struct par_40 : FSM::State {};
struct regions_par_40 : FSM::State {};
struct seq_62 : FSM::State {};
struct S121 : FSM::State {};
struct S122 : FSM::State {};
struct seq_63 : FSM::State {};
struct S123 : FSM::State {};
struct S124 : FSM::State {};
struct join_par_40 : FSM::State {};
struct seq_64 : FSM::State {};
struct S125 : FSM::State {};
struct S126 : FSM::State {};
struct par_41 : FSM::State {};
struct regions_par_41 : FSM::State {};
struct seq_65 : FSM::State {};
struct S127 : FSM::State {};
struct S128 : FSM::State {};
struct seq_66 : FSM::State {};
struct S129 : FSM::State {};
struct S130 : FSM::State {};
struct join_par_41 : FSM::State {};
struct join_exc_39 : FSM::State {};
struct join_exc_37 : FSM::State {};
struct join_exc_35 : FSM::State {};
struct S131 : FSM::State {};
struct seq_67 : FSM::State {};
struct exc_42 : FSM::State {};
struct split_exc_42 : FSM::State {};
struct seq_68 : FSM::State {};
struct S132 : FSM::State {};
struct S133 : FSM::State {};
struct par_43 : FSM::State {};
struct regions_par_43 : FSM::State {};
struct S134 : FSM::State {};
struct S135 : FSM::State {};
struct join_par_43 : FSM::State {};
struct seq_69 : FSM::State {};
struct S136 : FSM::State {};
struct S137 : FSM::State {};
struct par_44 : FSM::State {};
struct regions_par_44 : FSM::State {};
struct S138 : FSM::State {};
struct S139 : FSM::State {};
struct join_par_44 : FSM::State {};
struct join_exc_42 : FSM::State {};
struct join_exc_34 : FSM::State {};
struct join_exc_22 : FSM::State {};
struct exc_45 : FSM::State {};
struct split_exc_45 : FSM::State {};
struct seq_70 : FSM::State {};
struct exc_46 : FSM::State {};
struct split_exc_46 : FSM::State {};
struct seq_71 : FSM::State {};
struct S140 : FSM::State {};
struct par_47 : FSM::State {};
struct regions_par_47 : FSM::State {};
struct S141 : FSM::State {};
struct S142 : FSM::State {};
struct join_par_47 : FSM::State {};
struct seq_72 : FSM::State {};
struct S143 : FSM::State {};
struct join_exc_46 : FSM::State {};
struct join_exc_45 : FSM::State {};
struct join_exc_21 : FSM::State {};
struct S144 : FSM::State {};
struct exc_48 : FSM::State {};
struct split_exc_48 : FSM::State {};
struct seq_73 : FSM::State {};
struct S145 : FSM::State {};
struct join_exc_48 : FSM::State {};
struct exc_49 : FSM::State {};
struct split_exc_49 : FSM::State {};
struct seq_74 : FSM::State {};
struct S146 : FSM::State {};
struct S147 : FSM::State {};
struct join_exc_49 : FSM::State {};
struct S148 : FSM::State {};
struct S149 : FSM::State {};
struct S150 : FSM::State {};
struct exc_50 : FSM::State {};
struct split_exc_50 : FSM::State {};
struct seq_75 : FSM::State {};
struct exc_51 : FSM::State {};
struct split_exc_51 : FSM::State {};
struct seq_76 : FSM::State {};
struct S151 : FSM::State {};
struct join_exc_51 : FSM::State {};
struct S152 : FSM::State {};
struct seq_77 : FSM::State {};
struct exc_52 : FSM::State {};
struct split_exc_52 : FSM::State {};
struct seq_78 : FSM::State {};
struct S153 : FSM::State {};
struct join_exc_52 : FSM::State {};
struct join_exc_50 : FSM::State {};
struct S_end : FSM::State {};

//#pragma endregion

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 394, "");
static_assert(FSM::Instance::Info::REGION_COUNT  == 150, "");
static_assert(FSM::Instance::Info::COMPO_COUNT   == 131, "");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 352, "");
static_assert(FSM::Instance::Info::ORTHO_COUNT   ==  19, "");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  19, "");

////////////////////////////////////////////////////////////////////////////////

void step00(FSM::Instance& machine) {
	assertActive(machine, {
		FSM::stateId<S_start>(),
		FSM::stateId<S0     >(),
	});

	assertResumable(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step01(FSM::Instance& machine) {
	machine.immediateChangeTo<S1>();

	assertActive(machine, {
		FSM::stateId<S_start>(),
		FSM::stateId<S1     >(),
	});

	assertResumable(machine, {
		FSM::stateId<S0     >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step02(FSM::Instance& machine) {
	machine.immediateChangeTo<exc_1>();

	assertActive(machine, {
		FSM::stateId<S_start    >(),
		FSM::stateId<exc_1      >(),
		FSM::stateId<split_exc_1>(),
	});

	assertResumable(machine, {
		FSM::stateId<S1     >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step03(FSM::Instance& machine) {
	machine.immediateChangeTo<seq_9>();

	assertActive(machine, {
		FSM::stateId<S_start    >(),
		FSM::stateId<exc_1      >(),
		FSM::stateId<seq_9      >(),
		FSM::stateId<S19        >(),
	});

	assertResumable(machine, {
		FSM::stateId<S1         >(),
		FSM::stateId<split_exc_1>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step04(FSM::Instance& machine) {
	machine.immediateChangeTo<join_exc_1>();

	assertActive(machine, {
		FSM::stateId<S_start    >(),
		FSM::stateId<exc_1      >(),
		FSM::stateId<join_exc_1 >(),
	});

	assertResumable(machine, {
		FSM::stateId<S1         >(),
		FSM::stateId<seq_9      >(),
		FSM::stateId<S19        >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step05(FSM::Instance& machine) {
	machine.immediateChangeTo<S20>();

	assertActive(machine, {
		FSM::stateId<S_start    >(),
		FSM::stateId<S20        >(),
	});

	assertResumable(machine, {
		FSM::stateId<exc_1      >(),
		FSM::stateId<S19        >(),
		FSM::stateId<join_exc_1 >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step55(FSM::Instance& machine) {
	machine.immediateChangeTo<join_exc_52>();

	assertActive(machine, {
		FSM::stateId<S_start    >(),
		FSM::stateId<exc_50     >(),
		FSM::stateId<seq_77     >(),
		FSM::stateId<exc_52     >(),
		FSM::stateId<join_exc_52>(),
	});

	assertResumable(machine, {
		FSM::stateId<S19        >(),
		FSM::stateId<join_exc_1 >(),
		FSM::stateId<S20        >(),
	});
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Stress Size") {
	LoggerT<Config> logger;

	state_info_t si;
	std::shared_ptr<state_info_t> psi = std::make_shared<state_info_t>(si);

	FSM::Instance machine{psi, &logger};

	step00(machine);
	step01(machine);
	step02(machine);
	step03(machine);
	step04(machine);
	step05(machine);

	step55(machine);
}

////////////////////////////////////////////////////////////////////////////////

}

#endif
